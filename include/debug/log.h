#pragma once

#include <stdio.h>
#include <stdarg.h>

#include <options.h>

static inline void log_printf(const char * restrict fmt, ...) {
    if (options.log_enable) {
        va_list args;
        va_start(args, fmt);

        if (options.color_enable) printf("\033[36;1m");
        vprintf(fmt, args);
        if (options.color_enable) printf("\033[0m");
        fflush(stdout);

        va_end(args);
    }
}

static inline void log_putc(char c) {
    if (options.color_enable) printf("\033[36;1m");
    putc(c, stdout);
    if (options.color_enable) printf("\033[0m");
}

static inline void log_print_escaped(const char * restrict str) {
    if (options.log_enable) {
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (str[i] == '\a') log_printf("\\a");
            else if (str[i] == '\b') log_printf("\\b");
            else if (str[i] == '\f') log_printf("\\f");
            else if (str[i] == '\n') log_printf("\\n");
            else if (str[i] == '\r') log_printf("\\r");
            else if (str[i] == '\t') log_printf("\\t");
            else if (str[i] == '\v') log_printf("\\v");
            else if (str[i] == '\\') log_printf("\\\\");
            else if (str[i] == '\'') log_printf("\\'");
            else if (str[i] == '\"') log_printf("\\\"");
            else if (str[i] == '\?') log_printf("\\?");
            else log_putc(str[i]);
        }
    }
}

static inline void debug_printf(const char * restrict fmt, ...) {
    if (options.debug_enable) {
        va_list args;
        va_start(args, fmt);

        if (options.color_enable) printf("\033[35m");
        vprintf(fmt, args);
        if (options.color_enable) printf("\033[0m");
        fflush(stdout);

        va_end(args);
    }
}

static inline void debug_putc(char c) {
    if (options.color_enable) printf("\033[35m");
    putc(c, stdout);
    if (options.color_enable) printf("\033[0m");
}