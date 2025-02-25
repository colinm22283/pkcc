#pragma once

#include <stdbool.h>

static inline bool is_number_postfix_char(char c) {
    return
        c == 'u' ||
        c == 'l' ||
        c == 'U' ||
        c == 'L';
}