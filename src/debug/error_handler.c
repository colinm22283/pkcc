#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <debug/error_handler.h>

__attribute__((noreturn)) void fatal_error(const char * restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);

    printf("\033[31;1mERROR:\n");
    vprintf(fmt, args);
    printf("\033[0m\n");

    va_end(args);

    exit(1);
}