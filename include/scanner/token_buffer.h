#pragma once

#include <stddef.h>

#include <scanner/token.h>

#define TOKEN_BUFFER_DEFAULT_SIZE (1024)

typedef struct {
    size_t token_count, token_capacity;
    token_t * tokens;
} token_buffer_t;

void token_buffer_init(token_buffer_t * tb);
void token_buffer_free(token_buffer_t * tb);

token_t * token_buffer_push(token_buffer_t * tb, token_type_t type);
