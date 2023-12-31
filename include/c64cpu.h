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
#ifndef _c64cpu_h_
#define _c64cpu_h_
#include <c64mm.h>
#include <c64consts.h>
#include <c64utils.h>
#include <c64mem.h>
#include <c64instructions.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

struct c64cpu
{
    c64mm_t *mm;
    c64dev_t *registers;
    char flags;
    char *regNames[REG_COUNT];
    size_t stackFrameSize;
    uint64_t interruptVectorAddress;
};

c64cpu_t *c64cpu_create(c64mm_t *mm, uint64_t interruptVectorAddress);
c64dev_t *c64cpu_createRegisters(c64cpu_t *cpu);
void c64cpu_destroy(c64cpu_t *cpu);

uint64_t c64cpu_mapRegisterToOffset(c64cpu_t *cpu, char *regName);
size_t _c16cpu_getRegisterIndex(c64cpu_t *cpu, char *regName);
uint64_t c64cpu_getRegister(c64cpu_t *cpu, char *regName);
void c64cpu_setRegister(c64cpu_t *cpu, char *regName, uint64_t value);

// Sets or clears a flag
void c64cpu_setFlag(c64cpu_t *cpu, char value, char flag);
char c64cpu_getFlag(c64cpu_t *cpu, char flag);

uint8_t c64cpu_fetch(c64cpu_t *cpu);
uint16_t c64cpu_fetch16(c64cpu_t *cpu);
uint32_t c64cpu_fetch32(c64cpu_t *cpu);
uint64_t c64cpu_fetch64(c64cpu_t *cpu);

void c64cpu_push(c64cpu_t *cpu, uint64_t value);
void c64cpu_push32(c64cpu_t *cpu, uint32_t value);
void c64cpu_push16(c64cpu_t *cpu, uint16_t value);
void c64cpu_push8(c64cpu_t *cpu, uint8_t value);
uint64_t c64cpu_pop(c64cpu_t *cpu);
uint32_t c64cpu_pop32(c64cpu_t *cpu);
uint16_t c64cpu_pop16(c64cpu_t *cpu);
uint8_t c64cpu_pop8(c64cpu_t *cpu);

void c64cpu_pushState(c64cpu_t *cpu);
void c64cpu_popState(c64cpu_t *cpu);

size_t c64cpu_fetchRegisterIndex(c64cpu_t *cpu);

void c64cpu_handleInterrupt(c64cpu_t *cpu, uint16_t interrupt);

uint16_t c64cpu_execute(c64cpu_t *cpu, uint16_t opcode);
void c64cpu_run(c64cpu_t *cpu, char debug);

void c64cpu_debug(c64cpu_t *cpu);
void c64cpu_viewMemoryAt(c64cpu_t *cpu, uint64_t address, size_t size);
void c64cpu_viewMemoryAtWithHighlightedByte(c64cpu_t *cpu, uint64_t address, size_t size, uint64_t highlightedByteAddress);
uint16_t c64cpu_step(c64cpu_t *cpu);
void c64cpu_attachDebugger(c64cpu_t *cpu, void (*debugger)(c64cpu_t *cpu));

#endif // _c64cpu_h_