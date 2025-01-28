#pragma once

#include <stddef.h>

#include <scanner/token.h>

#define KEYWORD_STRINGS_COUNT (36)
#define PUNCTUATION_STRINGS_COUNT (46)

typedef struct {
    const char * string;
    size_t length;
    token_data_keyword_type_t keyword;
} keyword_string_entry_t;

extern keyword_string_entry_t keyword_strings[KEYWORD_STRINGS_COUNT];

typedef struct {
    const char * string;
    size_t length;
    token_data_punctuation_type_t punctuation;
} punctuation_string_entry_t;

extern punctuation_string_entry_t punctuation_strings[PUNCTUATION_STRINGS_COUNT];