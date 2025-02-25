#pragma once

#include <stdbool.h>

static inline bool is_number_char(char c) {
    return c >= '0' && c <= '9';
}