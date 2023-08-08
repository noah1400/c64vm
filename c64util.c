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