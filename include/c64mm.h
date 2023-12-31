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
#ifndef _c64mm_h_
#define _c64mm_h_
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <c64utils.h>
#include <c64cpu.h>

struct DeviceDriver
{
    char name[32];
    void *data;
    size_t dataSize;
    c64cpu_t *cpu;
    uint64_t (*getUint64)(c64dev_t *device, uint64_t address);
    uint32_t (*getUint32)(c64dev_t *device, uint64_t address);
    uint16_t (*getUint16)(c64dev_t *device, uint64_t address);
    uint8_t (*getUint8)(c64dev_t *device, uint64_t address);
    void (*setUint64)(c64dev_t *device, uint64_t address, uint64_t value);
    void (*setUint32)(c64dev_t *device, uint64_t address, uint32_t value);
    void (*setUint16)(c64dev_t *device, uint64_t address, uint16_t value);
    void (*setUint8)(c64dev_t *device, uint64_t address, uint8_t value);

    // The destroy function if defined will be called when the memory map is destroyed.
    // And needs to call free on the device itself.
    // If not defined the device will be freed by the memory map.
    // This is useful if the user wants to add additional behavior on destroy.
    void (*destroy)(c64dev_t *device);
};

struct MemoryMapRegion
{
    c64dev_t *device;
    uint64_t start;
    uint64_t end;
    char remap;
};

struct MemoryMap
{
    c64mmr_t **regions;
    uint64_t count;
};

c64mm_t *c64mm_create();
void c64mm_setCPU(c64mm_t *mm, c64cpu_t *cpu);
void c64mm_destroy(c64mm_t *mm);
void c64mm_map(c64mm_t *mm, c64dev_t *device, uint64_t start, uint64_t end, char remap);

c64mmr_t *c64mm_findRegion(c64mm_t *mm, uint64_t address);
uint64_t c64mm_getUint64(c64mm_t *mm, uint64_t address);
uint32_t c64mm_getUint32(c64mm_t *mm, uint64_t address);
uint16_t c64mm_getUint16(c64mm_t *mm, uint64_t address);
uint8_t c64mm_getUint8(c64mm_t *mm, uint64_t address);
void c64mm_setUint64(c64mm_t *mm, uint64_t address, uint64_t value);
void c64mm_setUint32(c64mm_t *mm, uint64_t address, uint32_t value);
void c64mm_setUint16(c64mm_t *mm, uint64_t address, uint16_t value);
void c64mm_setUint8(c64mm_t *mm, uint64_t address, uint8_t value);

void c16mm_print(c64mm_t *mm);

#endif // _c64mm_h_