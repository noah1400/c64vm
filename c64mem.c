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
#include <c64mem.h>

c64dev_t *c64mem_createDevice(size_t size, c64cpu_t *cpu)
{
    c64dev_t *device = malloc(sizeof(c64dev_t));
    if (device == NULL)
    {
        error("c64mem_createDevice: malloc failed\n");
    }
    device->getUint64 = c64mem_getUint64;
    device->getUint32 = c64mem_getUint32;
    device->getUint16 = c64mem_getUint16;
    device->getUint8 = c64mem_getUint8;
    device->setUint64 = c64mem_setUint64;
    device->setUint32 = c64mem_setUint32;
    device->setUint16 = c64mem_setUint16;
    device->setUint8 = c64mem_setUint8;
    device->destroy = c64mem_destroy;
    device->data = c64mem_createMemory(size);
    device->dataSize = size;
    device->cpu = cpu;

    strcpy(device->name, "Memory");

    return device;
}

void *c64mem_createMemory(size_t size)
{
    void *memory = malloc(size);
    if (memory == NULL)
    {
        error("c64mem_createMemory: malloc failed\n");
    }

    // Initialize memory to zero
    memset(memory, 0, size);
    return memory;
}

uint64_t c64mem_getUint64(c64dev_t *device, uint64_t address)
{
    // Check if address is out of bounds
    if (address + sizeof(uint64_t) > device->dataSize)
    {
        error("c64mem_getUint64: address out of bounds\n");
    }
    uint64_t res;
    memcpy(&res, (uint8_t *)(device->data) + address, sizeof(uint64_t));
    return res;
}

uint32_t c64mem_getUint32(c64dev_t *device, uint64_t address)
{
    // Check if address is out of bounds
    if (address + sizeof(uint32_t) > device->dataSize)
    {
        error("c64mem_getUint32: address out of bounds\n");
    }
    uint32_t res;
    memcpy(&res, (uint8_t *)(device->data) + address, sizeof(uint32_t));
    return res;
}

uint16_t c64mem_getUint16(c64dev_t *device, uint64_t address)
{
    // Check if address is out of bounds
    if (address + sizeof(uint16_t) > device->dataSize)
    {
        error("c64mem_getUint16: address out of bounds\n");
    }
    uint16_t res;
    memcpy(&res, (uint8_t *)(device->data) + address, sizeof(uint16_t));
    return res;
}

uint8_t c64mem_getUint8(c64dev_t *device, uint64_t address)
{
    // Check if address is out of bounds
    if (address + sizeof(uint8_t) > device->dataSize)
    {
        error("c64mem_getUint8: address out of bounds\n");
    }
    uint8_t res;
    memcpy(&res, (uint8_t *)(device->data) + address, sizeof(uint8_t));
    return res;
}

void c64mem_setUint64(c64dev_t *device, uint64_t address, uint64_t value)
{
    // Check if address is out of bounds
    if (address + sizeof(uint64_t) > device->dataSize)
    {
        error("c64mem_setUint64: address out of bounds\n");
    }
    memcpy((uint8_t *)(device->data) + address, &value, sizeof(uint64_t));
}

void c64mem_setUint32(c64dev_t *device, uint64_t address, uint32_t value)
{
    // Check if address is out of bounds
    if (address + sizeof(uint32_t) > device->dataSize)
    {
        error("c64mem_setUint32: address out of bounds\n");
    }
    memcpy((uint8_t *)(device->data) + address, &value, sizeof(uint32_t));
}

void c64mem_setUint16(c64dev_t *device, uint64_t address, uint16_t value)
{
    // Check if address is out of bounds
    if (address + sizeof(uint16_t) > device->dataSize)
    {
        error("c64mem_setUint16: address out of bounds\n");
    }
    memcpy((uint8_t *)(device->data) + address, &value, sizeof(uint16_t));
}

void c64mem_setUint8(c64dev_t *device, uint64_t address, uint8_t value)
{
    // Check if address is out of bounds
    if (address + sizeof(uint8_t) > device->dataSize)
    {
        error("c64mem_setUint8: address out of bounds\n");
    }
    memcpy((uint8_t *)(device->data) + address, &value, sizeof(uint8_t));
}

void c64mem_destroy(c64dev_t *device)
{
    free(device->data);
    free(device);
}