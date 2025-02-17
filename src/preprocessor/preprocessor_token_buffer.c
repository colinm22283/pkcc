#include <alloc.h>
#include <preprocessor/preprocessor_token_buffer.h>

#define PREPROCESSOR_TOKEN_BUFFER_INITIAL_SIZE (256)

void preprocessor_token_buffer_init(preprocessor_token_buffer_t * buffer) {
    buffer->size = 0;
    buffer->capacity = PREPROCESSOR_TOKEN_BUFFER_INITIAL_SIZE;

    buffer->tokens = pkcc_alloc(buffer->capacity * sizeof(preprocessor_token_t));
}

void preprocessor_token_buffer_free(preprocessor_token_buffer_t * buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        if (buffer->tokens[i].type == PREPROCESSOR_TOKEN_TYPE_SYMBOL) {
            if (buffer->tokens[i].symbol.args != NULL) {
                pkcc_free(buffer->tokens[i].symbol.args);
            }
        }
    }

    pkcc_free(buffer->tokens);
}

preprocessor_token_t * preprocessor_token_buffer_push(preprocessor_token_buffer_t * buffer) {
    preprocessor_token_t * token = &buffer->tokens[buffer->size++];

    if (buffer->size == buffer->capacity) {
        buffer->capacity *= 2;

        buffer->tokens = pkcc_realloc(buffer->tokens, buffer->capacity * sizeof(preprocessor_token_t));
    }

    return token;
}