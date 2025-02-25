#pragma once

#include <stdbool.h>

static inline bool case_insensitive_compare(char input, char lower) {
    return input == lower || input == lower + 'A' - 'a';
}