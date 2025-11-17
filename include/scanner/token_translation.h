#pragma once

#include <stddef.h>

#include <scanner/token.h>

#define KEYWORD_TRANSLATIONS_COUNT (37)
#define PUNCTUATION_TRANSLATIONS_COUNT (48)

typedef struct {
    const char * string;
    size_t length;
    token_data_keyword_type_t keyword;
} keyword_translation_entry_t;

extern keyword_translation_entry_t keyword_translations[KEYWORD_TRANSLATIONS_COUNT];

typedef struct {
    const char * string;
    size_t length;
    token_data_punctuation_type_t punctuation;
} punctuation_translation_entry_t;

extern punctuation_translation_entry_t punctuation_translations[PUNCTUATION_TRANSLATIONS_COUNT];