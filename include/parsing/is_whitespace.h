#pragma once

#include <stdbool.h>

static inline bool is_whitespace(char c) {
    return c == ' ' || c == '\t';
}