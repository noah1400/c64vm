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
#ifndef _c64vm_h_
#define _c64vm_h_

#include <stdint.h>
#include <stdlib.h>
#include <c64mem.h>
#include <c64cpu.h>
#include <c64consts.h>
#include <c64mm.h>
#include <c64instructions.h>
#include <c64utils.h>

void c64vm_run();

#endif // _c64vm_h_