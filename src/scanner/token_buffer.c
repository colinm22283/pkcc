#include <stdlib.h>

#include <alloc.h>
#include <scanner/token_buffer.h>

void token_buffer_init(token_buffer_t * tb) {
    log_printf("Initializing token buffer\n");

    tb->token_count = 0;
    tb->token_capacity = TOKEN_BUFFER_DEFAULT_SIZE;
    tb->tokens = pkcc_alloc(tb->token_capacity * sizeof(token_t));
}

void token_buffer_free(token_buffer_t * tb) {
    log_printf("Freeing token buffer\n");

    for (size_t i = 0; i < tb->token_count; i++) {
        token_free(&tb->tokens[i]);
    }

    pkcc_free(tb->tokens);
}

token_t * token_buffer_push(token_buffer_t * tb, token_type_t type) {
    log_printf("Pushing token to buffer\n");

    token_t * token = &tb->tokens[tb->token_count++];

    token_init(token, type);

    if (tb->token_count == tb->token_capacity) {
        tb->token_capacity *= 2;

        tb->tokens = pkcc_realloc(tb->tokens, tb->token_capacity);
    }

    return token;
}