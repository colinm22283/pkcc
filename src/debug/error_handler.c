#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <debug/error_handler.h>

__attribute__((noreturn)) void fatal_error(const char * restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "\033[31;1mERROR:\n");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\033[0m");

    va_end(args);

    exit(1);
}