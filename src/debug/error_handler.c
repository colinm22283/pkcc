#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <main.h>
#include <options.h>
#include <debug/error_handler.h>

__NORETURN void fatal_error(const char * restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);

    if (options.color_enable) fprintf(stderr, "\033[31;1mERROR:\n");
    else fprintf(stderr, "ERROR:\n");
    vfprintf(stderr, fmt, args);
    if (options.color_enable) fprintf(stderr, "\033[0m");

    va_end(args);

    exit_and_free(1);
}