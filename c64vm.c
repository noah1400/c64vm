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