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
#ifndef _c64utils_h_
#define _c64utils_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <c64consts.h>
#ifdef _WIN32
#include <Windows.h>
#define SLEEP_MS(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP_MS(ms) usleep(ms * 1000)
#endif

void error(const char *msg, ...);
void warning(const char *msg, ...);
void info(const char *msg, ...);
void debug(const char *msg, ...);

void out(const char *msg, ...); // Same as printf, but with a newline at the end

#endif // _c64utils_h_