#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

int xstrncpy(char *dest, const char *src, size_t destsize);
int xstrncat(char *dest, const char *src, size_t destsize);
void fatal(char *format, ...);

#endif
