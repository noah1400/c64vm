#include <c64mm.h>

c64mm_t *c64mm_create()
{
    c64mm_t *c64mm = (c64mm_t *)malloc(sizeof(c64mm_t));
    if (c64mm == NULL)
    {
        error("c64mm_create: malloc failed\n");
    }
    c64mm->count = 0;
    c64mm->regions = NULL;
    return c64mm;
}

void c64mm_setCPU(c64mm_t *mm, c64cpu_t *cpu)
{
    for (int i = 0; i < mm->count; i++)
    {
        mm->regions[i]->device->cpu = cpu;
    }
}

void c64mm_destroy(c64mm_t *mm)
{
    for (int i = 0; i < mm->count; i++)
    {
        c64dev_t *dev = mm->regions[i]->device;
        if (dev != NULL)
        {
            if (dev->destroy != NULL)
            {
                dev->destroy(dev);
            }
            else
            {
                warning("c64mm_destroy: no destroy function for device %s\n", dev->name);
                if (dev->data != NULL)
                    free(dev->data);
                free(dev);
            }
        }
        free(mm->regions[i]);
    }
    free(mm->regions);
    free(mm);
}

void c64mm_map(c64mm_t *mm, c64dev_t *device, uint64_t start, uint64_t end, char remap)
{
    if (start > end)
    {
        error("c64mm_map: start address %x is greater than end address %x\n", start, end);
    }
    c64mmr_t *region = (c64mmr_t *)malloc(sizeof(c64mmr_t));
    if (region == NULL)
    {
        error("c64mm_map: malloc failed\n");
    }
    region->device = device;
    region->start = start;
    region->end = end;
    region->remap = remap;

    mm->regions = (c64mmr_t **)realloc(mm->regions, (mm->count + 1) * sizeof(c64mmr_t *));
    for (int i = 0; i < mm->count; i++)
    {
        // Check if the new region overlaps with an existing region
        // check if either the start or end of the new region is within the existing region
        if ((region->start >= mm->regions[i]->start && region->start <= mm->regions[i]->end) ||
            (region->end >= mm->regions[i]->start && region->end <= mm->regions[i]->end))
        {
            error("c64mm_map: region %x - %x overlaps with existing region %x - %x which is mapped to %s\n", region->start, region->end, mm->regions[i]->start, mm->regions[i]->end, mm->regions[i]->device->name);
        }
    }
    for (int i = mm->count; i > 0; i--)
    {
        mm->regions[i] = mm->regions[i - 1];
    }

    mm->regions[0] = region;
    mm->count++;
}

c64mmr_t *c64mm_findRegion(c64mm_t *mm, uint64_t address)
{
    for (int i = 0; i < mm->count; i++)
    {
        if (address >= mm->regions[i]->start && address <= mm->regions[i]->end)
        {
            return mm->regions[i];
        }
    }
    warning("c64mm_findRegion: no region found for address %x\n", address);
    return NULL;
}

uint64_t c64mm_getUint64(c64mm_t *mm, uint64_t address)
{
    c64mmr_t *region = c64mm_findRegion(mm, address);
    if (region == NULL)
    {
        error("c64mm_getUint64: no region found for address %x\n", address);
    }
    uint64_t finalAddress = region->remap ? address - region->start : address;
    return region->device->getUint64(region->device, finalAddress);
}

uint32_t c64mm_getUint32(c64mm_t *mm, uint64_t address)
{
    c64mmr_t *region = c64mm_findRegion(mm, address);
    if (region == NULL)
    {
        error("c64mm_getUint32: no region found for address %x\n", address);
    }
    uint64_t finalAddress = region->remap ? address - region->start : address;
    return region->device->getUint32(region->device, finalAddress);
}

uint16_t c64mm_getUint16(c64mm_t *mm, uint64_t address)
{
    c64mmr_t *region = c64mm_findRegion(mm, address);
    if (region == NULL)
    {
        error("c64mm_getUint16: no region found for address %x\n", address);
    }
    uint64_t finalAddress = region->remap ? address - region->start : address;
    return region->device->getUint16(region->device, finalAddress);
}

uint8_t c64mm_getUint8(c64mm_t *mm, uint64_t address)
{
    c64mmr_t *region = c64mm_findRegion(mm, address);
    if (region == NULL)
    {
        error("c64mm_getUint8: no region found for address %x\n", address);
    }
    uint64_t finalAddress = region->remap ? address - region->start : address;
    return region->device->getUint8(region->device, finalAddress);
}

void c64mm_setUint64(c64mm_t *mm, uint64_t address, uint64_t value)
{
    c64mmr_t *region = c64mm_findRegion(mm, address);
    if (region == NULL)
    {
        error("c64mm_setUint64: no region found for address %x\n", address);
    }
    uint64_t finalAddress = region->remap ? address - region->start : address;
    region->device->setUint64(region->device, finalAddress, value);
}

void c64mm_setUint32(c64mm_t *mm, uint64_t address, uint32_t value)
{
    c64mmr_t *region = c64mm_findRegion(mm, address);
    if (region == NULL)
    {
        error("c64mm_setUint32: no region found for address %x\n", address);
    }
    uint64_t finalAddress = region->remap ? address - region->start : address;
    region->device->setUint32(region->device, finalAddress, value);
}

void c64mm_setUint16(c64mm_t *mm, uint64_t address, uint16_t value)
{
    c64mmr_t *region = c64mm_findRegion(mm, address);
    if (region == NULL)
    {
        error("c64mm_setUint16: no region found for address %x\n", address);
    }
    uint64_t finalAddress = region->remap ? address - region->start : address;
    region->device->setUint16(region->device, finalAddress, value);
}

void c64mm_setUint8(c64mm_t *mm, uint64_t address, uint8_t value)
{
    c64mmr_t *region = c64mm_findRegion(mm, address);
    if (region == NULL)
    {
        error("c64mm_setUint8: no region found for address %x\n", address);
    }
    uint64_t finalAddress = region->remap ? address - region->start : address;
    region->device->setUint8(region->device, finalAddress, value);
}

void c16mm_print(c64mm_t *mm)
{
    printf("Memory map:\n");
    for (int i = 0; i < mm->count; i++)
    {
        printf("  %08X - %08X: %s\n", mm->regions[i]->start, mm->regions[i]->end, mm->regions[i]->device->name);
    }
    printf("\n");
}