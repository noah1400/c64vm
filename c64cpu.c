/*
Copyright (c) 2023 Noah Scholz

This file is part of the c64vm project.

c64vm is free software: you can redistribute it and/or modify
it under the terms of the MIT License.

c64vm is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the MIT License for more details.

You should have received a copy of the MIT License
along with c64vm. If not, see <https://mit-license.org/>.
*/
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
    c64cpu_setFlag(cpu, FLAG_INTERRUPT, 1);
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
    return 0;
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

void c64cpu_setFlag(c64cpu_t *cpu, char flag, char value)
{
    if (value)
    {
        cpu->flags |= flag;
    }
    else
    {
        cpu->flags &= ~flag;
    }
}

char c64cpu_getFlag(c64cpu_t *cpu, char flag)
{
    return cpu->flags & flag;
}

uint8_t c64cpu_fetch(c64cpu_t *cpu)
{
    uint64_t nextInstructionAddress = c64cpu_getRegister(cpu, "IP");
    uint8_t instruction = c64mm_getUint8(cpu->mm, nextInstructionAddress);
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
    if (!c64cpu_getFlag(cpu, FLAG_INTERRUPT))
    {
        // 0 = 0 args. This is just to maintain our calling convention
        // If this were a software defined interrupt, the caller is expected
        // to supply any required data in registers
        c64cpu_push(cpu, 0);
        // Save the state
        c64cpu_pushState(cpu);
    }

    c64cpu_setFlag(cpu, FLAG_INTERRUPT, 1);

    // Jump to interrupt handler
    c64cpu_setRegister(cpu, "IP", interruptHandlerAddress);
}

uint16_t c64cpu_execute(c64cpu_t *cpu, uint16_t opcode)
{
    switch (opcode)
    {
    case LDI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64cpu_fetch64(cpu);
        c64mem_setUint64(cpu->registers, regIndex, value);
        return LDI;
    }
    case LDBI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint8_t value = c64cpu_fetch(cpu); 
        c64mem_setUint64(cpu->registers, regIndex, value); // still 64 bit register
        return LDBI;
    }
    case LDWI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint16_t value = c64cpu_fetch16(cpu);
        c64mem_setUint64(cpu->registers, regIndex, value); // still 64 bit register
        return LDWI;
    }
    case LDDI: // Load double word immediate 
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint32_t value = c64cpu_fetch32(cpu);
        c64mem_setUint64(cpu->registers, regIndex, value); // still 64 bit register
        return LDDI;
    }
    case LDM:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64cpu_fetch64(cpu);
        const uint64_t value = c64mm_getUint64(cpu->mm, address);
        c64mem_setUint64(cpu->registers, regIndex, value);
        return LDM;
    }
    case LDBM:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64cpu_fetch64(cpu);
        const uint8_t value = c64mm_getUint8(cpu->mm, address);
        c64mem_setUint64(cpu->registers, regIndex, value);
        return LDBM;
    }
    case LDWM:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64cpu_fetch64(cpu);
        const uint16_t value = c64mm_getUint16(cpu->mm, address);
        c64mem_setUint64(cpu->registers, regIndex, value);
        return LDWM;
    }
    case LDDM: // Load double word from memory
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64cpu_fetch64(cpu);
        const uint32_t value = c64mm_getUint32(cpu->mm, address);
        c64mem_setUint64(cpu->registers, regIndex, value);
        return LDDM;
    }
    case ST:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64cpu_fetch64(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        c64mm_setUint64(cpu->mm, address, value);
        return ST;
    }
    case STB:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64cpu_fetch64(cpu);
        // Add 7 to the address to get the last byte of the register
        const uint8_t value = c64mem_getUint8(cpu->registers, regIndex + sizeof(uint32_t) + sizeof(uint16_t) + sizeof(uint8_t));
        c64mm_setUint8(cpu->mm, address, value);
        return STB;
    }
    case STW:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64cpu_fetch64(cpu);
        // Add 6 to the address to get the last 2 bytes of the register
        const uint16_t value = c64mem_getUint16(cpu->registers, regIndex + sizeof(uint32_t) + sizeof(uint16_t));
        c64mm_setUint16(cpu->mm, address, value);
        return STW;
    }
    case STD: // Store double word
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64cpu_fetch64(cpu);
        // Add 4 to the address to get the last 4 bytes of the register
        const uint32_t value = c64mem_getUint32(cpu->registers, regIndex + sizeof(uint32_t));
        c64mm_setUint32(cpu->mm, address, value);
        return STD;
    }
    case TF:
    {
        const size_t regIndexFrom = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndexTo = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndexFrom);
        c64mem_setUint64(cpu->registers, regIndexTo, value);
        return TF;
    }
    case ADDI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64cpu_fetch64(cpu);
        const uint64_t currentValue = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t newValue = currentValue + value;

        // Flags
        const char isOverflow = newValue < currentValue;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return ADDI;
    }
    case SUBI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64cpu_fetch64(cpu);
        const uint64_t currentValue = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t newValue = currentValue - value;

        // Flags
        const char isOverflow = newValue > currentValue;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return SUBI;
    }
    case MULI: // unsigned
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64cpu_fetch64(cpu);
        const uint64_t currentValue = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t newValue = currentValue * value;

        // Flags
        const char isOverflow = newValue < currentValue;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return MULI;
    }
    case DIVI: // unsigned
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64cpu_fetch64(cpu);
        const uint64_t currentValue = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t newValue = currentValue / value;

        // Flags
        const char isOverflow = newValue > currentValue;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return DIVI;
    }
    case MODI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64cpu_fetch64(cpu);
        const uint64_t currentValue = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t newValue = currentValue % value;

        // Flags
        const char isOverflow = newValue > currentValue;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return MODI;
    }
    case MULIS: // signed
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const int64_t value = c64cpu_fetch64(cpu);
        const int64_t currentValue = c64mem_getUint64(cpu->registers, regIndex);
        const int64_t newValue = currentValue * value;

        // Flags
        const char isOverflow = newValue < currentValue;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue < 0;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return MULIS;
    }
    case DIVIS:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const int64_t value = c64cpu_fetch64(cpu);
        const int64_t currentValue = c64mem_getUint64(cpu->registers, regIndex);
        const int64_t newValue = currentValue / value;

        // Flags
        const char isOverflow = newValue > currentValue;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue < 0;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return DIVIS;
    }
    case ADD:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 + value2;

        // Flags
        const char isOverflow = newValue < value1;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return ADD;
    }
    case SUB:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 - value2;

        // Flags
        const char isOverflow = newValue > value1;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return SUB;
    }
    case MUL: // unsigned
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 * value2;

        // Flags
        const char isOverflow = newValue < value1;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return MUL;
    }
    case DIV: // unsigned
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 / value2;

        // Flags
        const char isOverflow = newValue > value1;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return DIV;
    }
    case MOD:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 % value2;

        // Flags
        const char isOverflow = newValue > value1;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return MOD;
    }
    case MULS: // signed
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const int64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const int64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const int64_t newValue = value1 * value2;

        // Flags
        const char isOverflow = newValue < value1;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return MULS;
    }
    case DIVS: // signed
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const int64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const int64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const int64_t newValue = value1 / value2;

        // Flags
        const char isOverflow = newValue > value1;
        const char isCarry = isOverflow;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_OVERFLOW, isOverflow);
        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return DIVS;
    }
    case ANDI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t imm = c64cpu_fetch64(cpu);
        const uint64_t newValue = value & imm;

        // Flags
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return ANDI;
    }
    case ORI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t imm = c64cpu_fetch64(cpu);
        const uint64_t newValue = value | imm;

        // Flags
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return ORI;
    }
    case XORI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t imm = c64cpu_fetch64(cpu);
        const uint64_t newValue = value ^ imm;

        // Flags
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return XORI;
    }
    case NOTI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t newValue = ~value;

        // Flags
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return NOTI;
    }
    case SHLI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t imm = c64cpu_fetch64(cpu);
        const uint64_t newValue = value << imm;

        // Flags
        const char isCarry = newValue < value;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return SHLI;
    }
    case SHRI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t imm = c64cpu_fetch64(cpu);
        const uint64_t newValue = value >> imm;

        // Flags
        const char isCarry = newValue > value;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return SHRI;
    }
    case RORI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t imm = c64cpu_fetch64(cpu);
        const uint64_t newValue = (value >> imm) | (value << (64 - imm));

        // Flags
        const char isCarry = newValue > value;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return RORI;
    }
    case ROLI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t imm = c64cpu_fetch64(cpu);
        const uint64_t newValue = (value << imm) | (value >> (64 - imm));

        // Flags
        const char isCarry = newValue < value;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return ROLI;
    }
    case AND:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 & value2;

        // Flags
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return AND;
    }
    case OR:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 | value2;

        // Flags
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return OR;
    }
    case XOR:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 ^ value2;

        // Flags
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return XOR;
    }
    case NOT:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t newValue = ~value;

        // Flags
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex, newValue);
        return NOT;
    }
    case SHL:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 << value2;

        // Flags
        const char isCarry = newValue < value1;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return SHL;
    }
    case SHR:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 >> value2;

        // Flags
        const char isCarry = newValue > value1;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return SHR;
    }
    case ROL:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = (value1 << value2) | (value1 >> (64 - value2));

        // Flags
        const char isCarry = newValue < value1;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return ROL;
    }
    case ROR:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = (value1 >> value2) | (value1 << (64 - value2));

        // Flags
        const char isCarry = newValue > value1;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);

        c64mem_setUint64(cpu->registers, regIndex1, newValue);
        return ROR;
    }
    case CMPI:
    {
        const size_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t value2 = c64cpu_fetch64(cpu);
        const uint64_t newValue = value1 - value2;

        // Flags
        const char isCarry = newValue > value1;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);
        return CMPI;
    }
    case CMP:
    {
        const size_t regIndex1 = c64cpu_fetchRegisterIndex(cpu);
        const size_t regIndex2 = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value1 = c64mem_getUint64(cpu->registers, regIndex1);
        const uint64_t value2 = c64mem_getUint64(cpu->registers, regIndex2);
        const uint64_t newValue = value1 - value2;

        // Flags
        const char isCarry = newValue > value1;
        const char isZero = newValue == 0;
        const char isNegative = newValue & 0x8000000000000000;

        c64cpu_setFlag(cpu, FLAG_CARRY, isCarry);
        c64cpu_setFlag(cpu, FLAG_ZERO, isZero);
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, isNegative);
        return CMP;
    }
    case JMP:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        c64cpu_setRegister(cpu, "IP", address);
        return JMP;
    }
    case JEQ:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (c64cpu_getFlag(cpu, FLAG_ZERO))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JEQ;
    }
    case JNE:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (!c64cpu_getFlag(cpu, FLAG_ZERO))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JNE;
    }
    case JGT:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (!c64cpu_getFlag(cpu, FLAG_ZERO) && !c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JGT;
    }
    case JLT:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JLT;
    }
    case JGE:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (!c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JGE;
    }
    case JLE:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (c64cpu_getFlag(cpu, FLAG_ZERO) || c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JLE;
    }
    case BRA:
    {
        const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
        c64cpu_push(cpu, retAdd);
        const uint64_t address = c64cpu_fetch64(cpu);
        c64cpu_setRegister(cpu, "IP", address);
        return BRA;
    }
    case BEQ:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (c64cpu_getFlag(cpu, FLAG_ZERO))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BEQ;
    }
    case BNE:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (!c64cpu_getFlag(cpu, FLAG_ZERO))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BNE;
    }
    case BGT:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (!c64cpu_getFlag(cpu, FLAG_ZERO) && !c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BGT;
    }
    case BLT:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BLT;
    }
    case BGE:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (!c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BGE;
    }
    case BLE:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        if (c64cpu_getFlag(cpu, FLAG_ZERO) || c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BLE;
    }
    case JMPR:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        c64cpu_setRegister(cpu, "IP", address);
        return JMPR;
    }
    case JEQR:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (c64cpu_getFlag(cpu, FLAG_ZERO))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JEQR;
    }
    case JNER:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (!c64cpu_getFlag(cpu, FLAG_ZERO))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JNER;
    }
    case JGTR:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (!c64cpu_getFlag(cpu, FLAG_ZERO) && !c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JGTR;
    }
    case JLTR:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JLTR;
    }
    case JGER:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (!c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JGER;
    }
    case JLER:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (c64cpu_getFlag(cpu, FLAG_ZERO) || c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            c64cpu_setRegister(cpu, "IP", address);
        }
        return JLER;
    }
    case BRAR:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
        c64cpu_push(cpu, retAdd);
        c64cpu_setRegister(cpu, "IP", address);
        return BRAR;
    }
    case BEQR:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (c64cpu_getFlag(cpu, FLAG_ZERO))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BEQR;
    }
    case BNER:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (!c64cpu_getFlag(cpu, FLAG_ZERO))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BNER;
    }
    case BGTR:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (!c64cpu_getFlag(cpu, FLAG_ZERO) && !c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BGTR;
    }
    case BLTR:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BLTR;
    }
    case BGER:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (!c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BGER;
    }
    case BLER:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        if (c64cpu_getFlag(cpu, FLAG_ZERO) || c64cpu_getFlag(cpu, FLAG_NEGATIVE))
        {
            const uint64_t retAdd = c64cpu_getRegister(cpu, "IP");
            c64cpu_push(cpu, retAdd);
            c64cpu_setRegister(cpu, "IP", address);
        }
        return BLER;
    }
    case RET:
    {
        const uint64_t retAdd = c64cpu_pop(cpu);
        c64cpu_setRegister(cpu, "IP", retAdd);
        return RET;
    }
    case PUSHI:
    {
        const uint64_t value = c64cpu_fetch64(cpu);
        c64cpu_push(cpu, value);
        return PUSHI;
    }
    case PUSH:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64mem_getUint64(cpu->registers, regIndex);
        c64cpu_push(cpu, value);
        return PUSH;
    }
    case POP:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t value = c64cpu_pop(cpu);
        c64mem_setUint64(cpu->registers, regIndex, value);
        return POP;
    }
    case CALL:
    {
        const uint64_t address = c64cpu_fetch64(cpu);
        c64cpu_pushState(cpu);
        c64cpu_setRegister(cpu, "IP", address);
        return CALL;
    }
    case CALLR:
    {
        const uint64_t regIndex = c64cpu_fetchRegisterIndex(cpu);
        const uint64_t address = c64mem_getUint64(cpu->registers, regIndex);
        c64cpu_pushState(cpu);
        c64cpu_setRegister(cpu, "IP", address);
        return CALLR;
    }
    case RTC:
    {
        c64cpu_popState(cpu);
        return RTC;
    }
    case CLC:
    {
        c64cpu_setFlag(cpu, FLAG_CARRY, 0);
        return CLC;
    }
    case SEC:
    {
        c64cpu_setFlag(cpu, FLAG_CARRY, 1);
        return SEC;
    }
    case CLZ:
    {
        c64cpu_setFlag(cpu, FLAG_ZERO, 0);
        return CLZ;
    }
    case SEZ:
    {
        c64cpu_setFlag(cpu, FLAG_ZERO, 1);
        return SEZ;
    }
    case CLN:
    {
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, 0);
        return CLN;
    }
    case SEN:
    {
        c64cpu_setFlag(cpu, FLAG_NEGATIVE, 1);
        return SEN;
    }
    case CLV:
    {
        c64cpu_setFlag(cpu, FLAG_OVERFLOW, 0);
        return CLV;
    }
    case SEV:
    {
        c64cpu_setFlag(cpu, FLAG_OVERFLOW, 1);
        return SEV;
    }
    case CLI:
    {
        c64cpu_setFlag(cpu, FLAG_INTERRUPT, 0);
        return CLI;
    }
    case SEI:
    {
        c64cpu_setFlag(cpu, FLAG_INTERRUPT, 1);
        return SEI;
    }
    case _INT:
    {
        const uint64_t value = c64cpu_fetch64(cpu);
        c64cpu_handleInterrupt(cpu, value);
        return _INT;
    }
    case RTI:
    {
        c64cpu_setFlag(cpu, FLAG_INTERRUPT, 0);
        c64cpu_popState(cpu);
        return RTI;
    }
    case NOP:
    {
        return NOP;
    }
    case HLT:
    {
        return HLT;
    }
    }

    c64cpu_debug(cpu);
    out("IP: 0x%08x", c64cpu_getRegister(cpu, "IP"));
    c64cpu_viewMemoryAtWithHighlightedByte(cpu, c64cpu_getRegister(cpu, "IP") - 8, 16, c64cpu_getRegister(cpu, "IP") - 1);

    error("Invalid opcode: 0x%04x", opcode);

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
    printf("0x%08llx: ", offset);
    for (size_t i = 0; i < size; i++)
    {
        printf("%02x ", c64mm_getUint8(cpu->mm, offset + i));
    }
    printf("\n");
}

void c64cpu_viewMemoryAtWithHighlightedByte(c64cpu_t *cpu, uint64_t offset, size_t size, uint64_t highlightedByte)
{
    printf("0x%08llx: ", offset);
    for (size_t i = 0; i < size; i++)
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
    printf("\n");
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