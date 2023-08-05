#ifndef _c64mem_h_
#define _c64mem_h_

#include <c64mm.h>
#include <stdint.h>

c64dev_t *c64mem_createDevice(size_t size);

void *c64mem_createMemory(size_t size);

#endif // _c64mem_h_