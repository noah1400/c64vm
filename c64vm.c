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
#include <c64vm.h>

// Just for now
// will be modified into a more complex interface
void c64vm_run()
{
    c64mm_t *mm = c64mm_create();
    c64cpu_t *cpu = c64cpu_create(mm, 0x10000000);
    c64dev_t *mem = c64mem_createDevice(0x0000000000ffffff, cpu);
    c64mm_map(mm, mem, 0xffffffffff000000, 0xffffffffffffffff, 1);

    c64cpu_destroy(cpu);
}