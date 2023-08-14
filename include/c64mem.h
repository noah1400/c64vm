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
#ifndef _c64mem_h_
#define _c64mem_h_

#include <c64mm.h>
#include <c64utils.h>
#include <c64cpu.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

c64dev_t *c64mem_createDevice(size_t size, c64cpu_t *cpu);

void *c64mem_createMemory(size_t size);

uint64_t c64mem_getUint64(c64dev_t *device, uint64_t address);
uint32_t c64mem_getUint32(c64dev_t *device, uint64_t address);
uint16_t c64mem_getUint16(c64dev_t *device, uint64_t address);
uint8_t c64mem_getUint8(c64dev_t *device, uint64_t address);

void c64mem_setUint64(c64dev_t *device, uint64_t address, uint64_t value);
void c64mem_setUint32(c64dev_t *device, uint64_t address, uint32_t value);
void c64mem_setUint16(c64dev_t *device, uint64_t address, uint16_t value);
void c64mem_setUint8(c64dev_t *device, uint64_t address, uint8_t value);

void c64mem_destroy(c64dev_t *device);

#endif // _c64mem_h_