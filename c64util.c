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
#include <c64utils.h>

void error(const char *msg, ...)
{
    if (LOG_LEVEL >= LOG_ERROR)
    {
        va_list args;
        va_start(args, msg);

        printf("ERROR: ");
        vprintf(msg, args);
        printf("\n");

        va_end(args);
    }
    exit(EXIT_FAILURE);
}

void warning(const char *msg, ...)
{
    if (LOG_LEVEL >= LOG_WARN)
    {
        va_list args;
        va_start(args, msg);

        printf("WARNING: ");
        vprintf(msg, args);
        printf("\n");

        va_end(args);
    }
}

void info(const char *msg, ...)
{
    if (LOG_LEVEL >= LOG_INFO)
    {
        va_list args;
        va_start(args, msg);

        printf("INFO: ");
        vprintf(msg, args);
        printf("\n");

        va_end(args);
    }
}

void debug(const char *msg, ...)
{
    if (LOG_LEVEL >= LOG_DEBUG)
    {
        va_list args;
        va_start(args, msg);

        printf("DEBUG: ");
        vprintf(msg, args);
        printf("\n");

        va_end(args);
    }
}

void out(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    vprintf(msg, args);
    printf("\n");

    va_end(args);
}