#pragma once

#include <stdbool.h>

static inline bool is_line_end(char c) {
    return c == '\n' || c == '\0';
}