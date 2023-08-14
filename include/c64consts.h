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
#ifndef _c64consts_h_
#define _c64consts_h_

#define FLAG_CARRY 0x01
#define FLAG_ZERO 0x02
#define FLAG_NEGATIVE 0x04
#define FLAG_OVERFLOW 0x08
#define FLAG_INTERRUPT 0x10

#define LOG_NONE 0
#define LOG_ERROR 1
#define LOG_WARN 2
#define LOG_INFO 3
#define LOG_DEBUG 4

#define LOG_LEVEL LOG_DEBUG // 0 = no logging, 1 = errors, 2 = warnings, 3 = info, 4 = debug

#define REG_COUNT 14

#define REG_IP 0
#define REG_ACC 1
#define REG_R1 2
#define REG_R2 3
#define REG_R3 4
#define REG_R4 5
#define REG_R5 6
#define REG_R6 7
#define REG_R7 8
#define REG_R8 9
#define REG_SP 10
#define REG_FP 11
#define REG_MB 12
#define REG_IM 13

#define MEMORY_SIZE 65536
#define c64cpu_speed 1000000

// Forward declarations
typedef struct c64cpu c64cpu_t;
typedef struct MemoryMap c64mm_t;
typedef struct MemoryMapRegion c64mmr_t;
typedef struct DeviceDriver c64dev_t;

#endif // _c64consts_h_