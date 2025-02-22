#pragma once

#include <stdbool.h>

static inline bool is_symbol_start_char(char c) {
    return (
        (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        c == '_'
    );
}

static inline bool is_symbol_char(char c) {
    return (
        is_symbol_start_char(c) ||
        (c >= '0' && c <= '9')
    );
}