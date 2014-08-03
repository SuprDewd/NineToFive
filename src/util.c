#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int xstrncpy(char *dest, const char *src, size_t destsize) {
    int srclen = strlen(src);
    if (srclen + 1 > destsize)
        return -1;

    strcpy(dest, src);
    return srclen;
}

int xstrncat(char *dest, const char *src, size_t destsize) {
    int destlen = strlen(dest);
    int srclen = strlen(src);
    if (srclen + destlen + 1 > destsize)
        return -1;

    strcpy(dest + destlen, src);
    return destlen + srclen;
}

void fatal(char *format, ...) {
    fprintf(stderr, "error: ");

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    fprintf(stderr, "\n");
    exit(1);
}

