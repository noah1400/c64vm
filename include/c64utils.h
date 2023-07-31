#ifndef _c64utils_h_
#define _c64utils_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <c64consts.h>

void    error(const char *msg, ...);
void    warning(const char *msg, ...);
void    info(const char *msg, ...);
void    debug(const char *msg, ...);

void    out(const char *msg, ...); // Same as printf, but with a newline at the end

#endif // _c64utils_h_