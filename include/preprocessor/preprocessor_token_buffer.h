#pragma once

#include <stddef.h>

typedef enum {
    PREPROCESSOR_TOKEN_TYPE_DIRECTIVE,
    PREPROCESSOR_TOKEN_TYPE_SYMBOL,
    PREPROCESSOR_TOKEN_TYPE_CONTENT,
} preprocessor_token_type_t;

typedef enum {
    PREPROCESSOR_TOKEN_DIRECTIVE_DEFINE,
    PREPROCESSOR_TOKEN_DIRECTIVE_UNDEF,
    PREPROCESSOR_TOKEN_DIRECTIVE_INCLUDE,
    PREPROCESSOR_TOKEN_DIRECTIVE_IF,
    PREPROCESSOR_TOKEN_DIRECTIVE_IFDEF,
} preprocessor_token_directive_type_t;

typedef struct {
    preprocessor_token_directive_type_t type;

    const char * content;
    size_t size;
} preprocessor_token_directive_t;

typedef struct {
    const char * symbol;
    size_t size;

    size_t arg_count;
    const char ** args;
} preprocessor_token_symbol_t;

typedef struct {
    const char * content;
    size_t size;
} preprocessor_token_content_t;

typedef struct {
    preprocessor_token_type_t type;

    union {
        preprocessor_token_directive_t directive;
        preprocessor_token_symbol_t symbol;
        preprocessor_token_content_t content;
    };
} preprocessor_token_t;

typedef struct {
    size_t size, capacity;
    preprocessor_token_t * tokens;
} preprocessor_token_buffer_t;

void preprocessor_token_buffer_init(preprocessor_token_buffer_t * buffer);
void preprocessor_token_buffer_free(preprocessor_token_buffer_t * buffer);

preprocessor_token_t * preprocessor_token_buffer_push(preprocessor_token_buffer_t * buffer);
