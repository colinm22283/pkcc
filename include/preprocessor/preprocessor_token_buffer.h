#pragma once

#include <stddef.h>

#include <file/file_name_registry.h>

typedef enum {
    PREPROCESSOR_TOKEN_TYPE_DIRECTIVE,
    PREPROCESSOR_TOKEN_TYPE_SYMBOL,
    PREPROCESSOR_TOKEN_TYPE_CONTENT,
    PREPROCESSOR_TOKEN_TYPE_STRING,
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

    char * content;
    size_t size;
} preprocessor_token_directive_t;

typedef struct {
    char * symbol;
    size_t size;

    size_t arg_count;
    size_t * args;
} preprocessor_token_symbol_t;

typedef struct {
    char * content;
    size_t size;
} preprocessor_token_content_t;

typedef struct {
    char * content;
    size_t size;
} preprocessor_token_string_t;

typedef struct {
    preprocessor_token_type_t type;

    union {
        preprocessor_token_directive_t directive;
        preprocessor_token_symbol_t symbol;
        preprocessor_token_content_t content;
        preprocessor_token_string_t string;
    };

    size_t line;
    file_name_entry_t * file_name;
} preprocessor_token_t;

typedef struct {
    size_t size, capacity;
    preprocessor_token_t * tokens;
} preprocessor_token_buffer_t;

void preprocessor_token_buffer_init(preprocessor_token_buffer_t * buffer);
void preprocessor_token_buffer_free(preprocessor_token_buffer_t * buffer);

preprocessor_token_t * preprocessor_token_buffer_push(preprocessor_token_buffer_t * buffer);

void preprocessor_token_directive_init(preprocessor_token_t * token, preprocessor_token_directive_type_t type, const char * content, size_t size);
void preprocessor_token_symbol_init(preprocessor_token_t * token, const char * symbol, size_t size);
void preprocessor_token_content_init(preprocessor_token_t * token, const char * content, size_t size);
void preprocessor_token_string_init(preprocessor_token_t * token, const char * content, size_t size);
void preprocessor_token_free(preprocessor_token_t * token);

void preprocessor_token_buffer_token_clone(preprocessor_token_t * dst, preprocessor_token_t * src);
