#include <c64cpu.h>

c64cpu_t *c64cpu_create(c64mm_t *mm, uint64_t interruptVectorAddress)
{
    c64cpu_t *cpu = (c64cpu_t *)malloc(sizeof(c64cpu_t));
    if (cpu == NULL)
    {
        error("c64cpu_create: malloc failed\n");
    }

    cpu->mm = mm;

    cpu->registers = c64cpu_createRegisters(cpu);

    cpu->regNames[REG_IP] = "IP";
    cpu->regNames[REG_ACC] = "ACC";
    cpu->regNames[REG_R1] = "R1";
    cpu->regNames[REG_R2] = "R2";
    cpu->regNames[REG_R3] = "R3";
    cpu->regNames[REG_R4] = "R4";
    cpu->regNames[REG_R5] = "R5";
    cpu->regNames[REG_R6] = "R6";
    cpu->regNames[REG_R7] = "R7";
    cpu->regNames[REG_R8] = "R8";
    cpu->regNames[REG_SP] = "SP";
    cpu->regNames[REG_FP] = "FP";
    cpu->regNames[REG_MB] = "MB";
    cpu->regNames[REG_IM] = "IM";

    cpu->flags = 0;

    cpu->interruptVectorAddress = interruptVectorAddress;
    cpu->isInInterruptHandler = 0;
    c64cpu_setRegister(cpu, "IM", 0xffffffff);

    c64cpu_setRegister(cpu, "SP", 0xffffffff - 1);
    c64cpu_setRegister(cpu, "FP", 0xffffffff - 1);

    cpu->stackFrameSize = 0;

    return cpu;
}

c64dev_t *c64cpu_createRegisters(c64cpu_t *cpu)
{
    return c64mem_createDevice(REG_COUNT * sizeof(uint64_t), cpu);
}

size_t _c16cpu_getRegisterIndex(c64cpu_t *cpu, char *regName)
{
    for (size_t i = 0; i < REG_COUNT; i++)
    {
        if (strcmp(cpu->regNames[i], regName) == 0)
        {
            return i;
        }
    }
    error("c16cpu_getRegisterIndex: unknown register name %s\n", regName);
}

uint64_t c64cpu_mapRegisterToOffset(c64cpu_t *cpu, char *regName)
{
    return _c16cpu_getRegisterIndex(cpu, regName) * sizeof(uint64_t);
}

uint64_t c64cpu_getRegister(c64cpu_t *cpu, char *regName)
{
    size_t offset = c64cpu_mapRegisterToOffset(cpu, regName);
    return c64mem_getUint64(cpu->registers, offset);
}

void c64cpu_setRegister(c64cpu_t *cpu, char *regName, uint64_t value)
{
    size_t offset = c64cpu_mapRegisterToOffset(cpu, regName);
    c64mem_setUint64(cpu->registers, offset, value);
}

uint8_t c64cpu_fetch(c64cpu_t *cpu)
{
    uint64_t nextInstructionAddress = c64cpu_getRegister(cpu, "IP");
    uint8_t instruction = c64mm_getUint16(cpu->mm, nextInstructionAddress);
    c64cpu_setRegister(cpu, "IP", nextInstructionAddress + sizeof(uint8_t));
    return instruction;
}

uint16_t c64cpu_fetch16(c64cpu_t *cpu)
{
    uint64_t nextInstructionAddress = c64cpu_getRegister(cpu, "IP");
    uint16_t instruction = c64mm_getUint16(cpu->mm, nextInstructionAddress);
    c64cpu_setRegister(cpu, "IP", nextInstructionAddress + sizeof(uint16_t));
    return instruction;
}

uint32_t c64cpu_fetch32(c64cpu_t *cpu)
{
    uint64_t nextInstructionAddress = c64cpu_getRegister(cpu, "IP");
    uint32_t instruction = c64mm_getUint32(cpu->mm, nextInstructionAddress);
    c64cpu_setRegister(cpu, "IP", nextInstructionAddress + sizeof(uint32_t));
    return instruction;
}

uint64_t c64cpu_fetch64(c64cpu_t *cpu)
{
    uint64_t nextInstructionAddress = c64cpu_getRegister(cpu, "IP");
    uint64_t instruction = c64mm_getUint64(cpu->mm, nextInstructionAddress);
    c64cpu_setRegister(cpu, "IP", nextInstructionAddress + sizeof(uint64_t));
    return instruction;
}

void c64cpu_push(c64cpu_t *cpu, uint64_t value)
{
    uint64_t sp = c64cpu_getRegister(cpu, "SP");
    c64mm_setUint64(cpu->mm, sp, value);
    c64cpu_setRegister(cpu, "SP", sp - sizeof(uint64_t));
}

void c64cpu_push32(c64cpu_t *cpu, uint32_t value)
{
    uint64_t sp = c64cpu_getRegister(cpu, "SP");
    c64mm_setUint32(cpu->mm, sp, value);
    c64cpu_setRegister(cpu, "SP", sp - sizeof(uint32_t));
}

void c64cpu_push16(c64cpu_t *cpu, uint16_t value)
{
    uint64_t sp = c64cpu_getRegister(cpu, "SP");
    c64mm_setUint16(cpu->mm, sp, value);
    c64cpu_setRegister(cpu, "SP", sp - sizeof(uint16_t));
}

void c64cpu_push8(c64cpu_t *cpu, uint8_t value)
{
    uint64_t sp = c64cpu_getRegister(cpu, "SP");
    c64mm_setUint8(cpu->mm, sp, value);
    c64cpu_setRegister(cpu, "SP", sp - sizeof(uint8_t));
}

uint64_t c64cpu_pop(c64cpu_t *cpu)
{
    uint64_t nextSpAddress = c64cpu_getRegister(cpu, "SP") + sizeof(uint64_t);
    c64cpu_setRegister(cpu, "SP", nextSpAddress);
    cpu->stackFrameSize -= sizeof(uint64_t);
    return c64mm_getUint64(cpu->mm, nextSpAddress);
}

uint32_t c64cpu_pop32(c64cpu_t *cpu)
{
    uint64_t nextSpAddress = c64cpu_getRegister(cpu, "SP") + sizeof(uint32_t);
    c64cpu_setRegister(cpu, "SP", nextSpAddress);
    cpu->stackFrameSize -= sizeof(uint32_t);
    return c64mm_getUint32(cpu->mm, nextSpAddress);
}

uint16_t c64cpu_pop16(c64cpu_t *cpu)
{
    uint64_t nextSpAddress = c64cpu_getRegister(cpu, "SP") + sizeof(uint16_t);
    c64cpu_setRegister(cpu, "SP", nextSpAddress);
    cpu->stackFrameSize -= sizeof(uint16_t);
    return c64mm_getUint16(cpu->mm, nextSpAddress);
}

uint8_t c64cpu_pop8(c64cpu_t *cpu)
{
    uint64_t nextSpAddress = c64cpu_getRegister(cpu, "SP") + sizeof(uint8_t);
    c64cpu_setRegister(cpu, "SP", nextSpAddress);
    cpu->stackFrameSize -= sizeof(uint8_t);
    return c64mm_getUint8(cpu->mm, nextSpAddress);
}

void c64cpu_pushState(c64cpu_t *cpu)
{
    c64cpu_push(cpu, c64cpu_getRegister(cpu, "R1"));
    c64cpu_push(cpu, c64cpu_getRegister(cpu, "R2"));
    c64cpu_push(cpu, c64cpu_getRegister(cpu, "R3"));
    c64cpu_push(cpu, c64cpu_getRegister(cpu, "R4"));
    c64cpu_push(cpu, c64cpu_getRegister(cpu, "R5"));
    c64cpu_push(cpu, c64cpu_getRegister(cpu, "R6"));
    c64cpu_push(cpu, c64cpu_getRegister(cpu, "R7"));
    c64cpu_push(cpu, c64cpu_getRegister(cpu, "R8"));
    c64cpu_push(cpu, c64cpu_getRegister(cpu, "IP"));
    c64cpu_push(cpu, cpu->stackFrameSize + sizeof(uint64_t));

    c64cpu_setRegister(cpu, "FP", c64cpu_getRegister(cpu, "SP"));
    cpu->stackFrameSize = 0;
}

void c64cpu_popState(c64cpu_t *cpu)
{
    uint64_t fpa = c64cpu_getRegister(cpu, "FP");
    c64cpu_setRegister(cpu, "SP", fpa);

    cpu->stackFrameSize = c64cpu_pop(cpu);
    const uint64_t sfs = cpu->stackFrameSize;

    c64cpu_setRegister(cpu, "IP", c64cpu_pop(cpu));
    c64cpu_setRegister(cpu, "R8", c64cpu_pop(cpu));
    c64cpu_setRegister(cpu, "R7", c64cpu_pop(cpu));
    c64cpu_setRegister(cpu, "R6", c64cpu_pop(cpu));
    c64cpu_setRegister(cpu, "R5", c64cpu_pop(cpu));
    c64cpu_setRegister(cpu, "R4", c64cpu_pop(cpu));
    c64cpu_setRegister(cpu, "R3", c64cpu_pop(cpu));
    c64cpu_setRegister(cpu, "R2", c64cpu_pop(cpu));
    c64cpu_setRegister(cpu, "R1", c64cpu_pop(cpu));

    const uint64_t nArgs = c64cpu_pop(cpu);
    for (uint64_t i = 0; i < nArgs; i++)
    {
        c64cpu_pop(cpu);
    }

    c64cpu_setRegister(cpu, "FP", fpa + sfs);
}

size_t c64cpu_fetchRegisterIndex(c64cpu_t *cpu)
{
    // Registers indexes are 1 byte long and reference a 64 bit register
    // Will always return a valid index even if the instruction is invalid
    return (c64cpu_fetch(cpu) % REG_COUNT) * sizeof(uint64_t);
}

void c64cpu_handleInterrupt(c64cpu_t *cpu, uint16_t value)
{
    const unsigned char interruptBit = value % 64;

    // If the interrupt is masked by the interrupt mask register
    // then do not enter the interrupt handler
    const unsigned char isUnmasked = (1 << interruptBit) & c64cpu_getRegister(cpu, "IM");
    if (!isUnmasked)
    {
        return;
    }

    // Calculate where in the interrupt vector we'll look
    const uint64_t interruptVectorAddress = cpu->interruptVectorAddress + (interruptBit * sizeof(uint64_t));
    // Get the address from the interrupt vector at that address
    const uint64_t interruptHandlerAddress = c64mm_getUint64(cpu->mm, interruptVectorAddress);

    // We only save the state if we're not already in an interrupt handler
    if (!cpu->isInInterruptHandler)
    {
        // 0 = 0 args. This is just to maintain our calling convention
        // If this were a software defined interrupt, the caller is expected
        // to supply any required data in registers
        c64cpu_push(cpu, 0);
        // Save the state
        c64cpu_pushState(cpu);
    }

    cpu->isInInterruptHandler = 1;

    // Jump to interrupt handler
    c64cpu_setRegister(cpu, "IP", interruptHandlerAddress);
}

uint16_t c64cpu_execute(c64cpu_t *cpu, uint16_t opcode)
{
    switch(opcode)
    {

    }

    

    return opcode;
}

uint16_t c64cpu_step(c64cpu_t *cpu)
{
    uint16_t opcode = c64cpu_fetch16(cpu);
    return c64cpu_execute(cpu, opcode);
}

void c64cpu_debug(c64cpu_t *cpu)
{
    out("Registers:");
    for (int i = 0; i < REG_COUNT; i++)
    {
        out("  %s: 0x%08x", cpu->regNames[i], c64cpu_getRegister(cpu, cpu->regNames[i]));
    }
    out("");
}

void c64cpu_viewMemoryAt(c64cpu_t *cpu, uint64_t offset, size_t size)
{
    printf("0x%08x: ", offset);
    for (int i = 0; i < size; i++)
    {
        printf("%02x ", c64mm_getUint8(cpu->mm, offset + i));
    }
    printf("\n");
}

void c64cpu_viewMemoryAtWithHighlightedByte(c64cpu_t *cpu, uint64_t offset, size_t size, uint64_t highlightedByte)
{
    printf("0x%08x: ", offset);
    for (int i = 0; i < size; i++)
    {
        if (offset + i == highlightedByte)
        {
            printf("\033[40m\033[91m");
        }
        printf("%02x ", c64mm_getUint8(cpu->mm, offset + i));
        if (offset + i == highlightedByte)
        {
            printf("\033[0m");
        }
    }
    printf("");
}

// Debug function needs to contain a c64cpu_step call
void c64cpu_attachDebugger(c64cpu_t *cpu, void (*debugger)(c64cpu_t *cpu))
{
    char input[256];
    while (1)
    {
        debugger(cpu);
        printf("\n");
        printf("enter s to step, c to continue, or q to quit, or a number to step that many times: ");
        fgets(input, 256, stdin);
        if (input[0] == 'q')
        {
            break;
        }
        if (input[0] > '0' && input[0] <= '9')
        {
            const int n = atoi(input);
            for (int i = 0; i < n; i++)
            {
                debugger(cpu);
            }
            continue;
        }
    }

    printf("exiting debugger\n");
}

void c64cpu_destroy(c64cpu_t *cpu)
{
    c64mm_destroy(cpu->mm);
    c64mem_destroy(cpu->registers);
    free(cpu);
}

void c64cpu_run(c64cpu_t *cpu, char debug)
{
#ifdef _WIN32
    // Windows
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
#else
    // Linux
    struct timespec start, end;
#endif

    double elapsed_ms, target_ms, sleep_ms;

#ifdef _WIN32
    QueryPerformanceFrequency(&frequency);
#endif

    while (1)
    {
#ifdef _WIN32
        QueryPerformanceCounter(&start);
#else
        clock_gettime(CLOCK_MONOTONIC, &start);
#endif

        uint16_t opcode = c64cpu_step(cpu);
        if (debug)
        {
            c64cpu_debug(cpu);
        }
        if (opcode == HLT)
        {
            break;
        }
#ifdef _WIN32
        QueryPerformanceCounter(&end);
        elapsed_ms = (end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;
#else
        clock_gettime(CLOCK_MONOTONIC, &end);
        elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0;
        elapsed_ms += (end.tv_nsec - start.tv_nsec) / 1000000.0;
#endif
        target_ms = 1000.0 / c64cpu_speed;

        sleep_ms = target_ms - elapsed_ms;
        if (sleep_ms > 0)
        {
            SLEEP_MS(sleep_ms);
        }
    }
}