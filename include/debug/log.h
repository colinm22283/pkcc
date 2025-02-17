#pragma once

#include <stdio.h>
#include <stdarg.h>

#include <options.h>

static inline void log_printf(const char * restrict fmt, ...) {
    if (options.log_enable) {
        va_list args;
        va_start(args, fmt);

        printf("\033[36;1m");
        vprintf(fmt, args);
        printf("\033[0m");
        fflush(stdout);

        va_end(args);
    }
}

static inline void debug_printf(const char * restrict fmt, ...) {
    if (options.debug_enable) {
        va_list args;
        va_start(args, fmt);

        printf("\033[35m");
        vprintf(fmt, args);
        printf("\033[0m");
        fflush(stdout);

        va_end(args);
    }
}