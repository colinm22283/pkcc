#include <alloc.h>
#include <preprocessor/preprocessor_token_buffer.h>

#define PREPROCESSOR_TOKEN_BUFFER_INITIAL_SIZE (4096)

void preprocessor_token_buffer_init(preprocessor_token_buffer_t * buffer) { // TODO this is bad
    buffer->size = 0;
    buffer->capacity = PREPROCESSOR_TOKEN_BUFFER_INITIAL_SIZE;

    buffer->tokens = pkcc_alloc(buffer->capacity * sizeof(preprocessor_token_t));
}

void preprocessor_token_buffer_free(preprocessor_token_buffer_t * buffer) {
    for (size_t i = 0; i < buffer->size; i++) {
        preprocessor_token_free(&buffer->tokens[i]);
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

void preprocessor_token_directive_init(preprocessor_token_t * token, preprocessor_token_directive_type_t type, const char * content, size_t size) {
    token->type = PREPROCESSOR_TOKEN_TYPE_DIRECTIVE;
    token->directive.type = type;
    token->directive.size = size;

    token->directive.content = pkcc_alloc(size);
    memcpy(token->directive.content, content, size);
}

void preprocessor_token_symbol_init(preprocessor_token_t * token, const char * symbol, size_t size) {
    token->type = PREPROCESSOR_TOKEN_TYPE_SYMBOL;
    token->symbol.size = size;
    token->symbol.args = NULL;
    token->symbol.arg_count = 0;

    token->symbol.symbol = pkcc_alloc(size);
    memcpy(token->symbol.symbol, symbol, size);
}

void preprocessor_token_content_init(preprocessor_token_t * token, const char * content, size_t size) {
    token->type = PREPROCESSOR_TOKEN_TYPE_CONTENT;
    token->content.size = size;

    token->content.content = pkcc_alloc(size);
    memcpy(token->content.content, content, size);
}

void preprocessor_token_string_init(preprocessor_token_t * token, const char * content, size_t size) {
    token->type = PREPROCESSOR_TOKEN_TYPE_STRING;
    token->string.size = size;

    token->string.content = pkcc_alloc(size);
    memcpy(token->string.content, content, size);
}

void preprocessor_token_free(preprocessor_token_t * token) {
    switch (token->type) {
        case PREPROCESSOR_TOKEN_TYPE_DIRECTIVE: {
            pkcc_free(token->directive.content);
        } break;

        case PREPROCESSOR_TOKEN_TYPE_SYMBOL: {
            pkcc_free(token->symbol.symbol);

            if (token->symbol.arg_count > 0) pkcc_free(token->symbol.args);
        } break;

        case PREPROCESSOR_TOKEN_TYPE_CONTENT: {
            pkcc_free(token->content.content);
        } break;

        case PREPROCESSOR_TOKEN_TYPE_STRING: {
            pkcc_free(token->string.content);
        } break;
    }
}

void preprocessor_token_buffer_token_clone(preprocessor_token_t * dst, preprocessor_token_t * src) {
    dst->type = src->type;

    dst->file_name = src->file_name;
    dst->line = src->line;

    switch (src->type) {
        case PREPROCESSOR_TOKEN_TYPE_DIRECTIVE: {
            dst->directive.type = src->directive.type;
            dst->directive.size = src->directive.size;

            dst->directive.content = pkcc_alloc(src->directive.size);
            memcpy(dst->directive.content, src->directive.content, src->directive.size);
        } break;

        case PREPROCESSOR_TOKEN_TYPE_SYMBOL: {
            dst->symbol.size = src->symbol.size;
            dst->symbol.arg_count = src->symbol.arg_count;

            dst->symbol.symbol = pkcc_alloc(src->symbol.size);
            memcpy(dst->symbol.symbol, src->symbol.symbol, src->symbol.size);

            if (src->symbol.arg_count == 0) {
                dst->symbol.args = NULL;
                dst->symbol.arg_count = 0;
            }
            else {
                dst->symbol.args = pkcc_alloc(src->symbol.arg_count * sizeof(size_t));
                memcpy(dst->symbol.args, src->symbol.args, src->symbol.arg_count * sizeof(size_t));
            }
        } break;

        case PREPROCESSOR_TOKEN_TYPE_CONTENT: {
            dst->content.size = src->content.size;

            dst->content.content = pkcc_alloc(src->content.size);
            memcpy(dst->content.content, src->content.content, src->content.size);
        } break;

        case PREPROCESSOR_TOKEN_TYPE_STRING: {
            dst->string.size = src->content.size;

            dst->string.content = pkcc_alloc(src->string.size);
            memcpy(dst->string.content, src->string.content, src->string.size);
        }
    }
}