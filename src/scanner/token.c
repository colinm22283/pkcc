#include <alloc.h>

#include <scanner/token.h>

void token_init(token_t * token, token_type_t type) {
    static const size_t token_sizes[] = {
        [TOKEN_TYPE_IDENTIFIER] = sizeof(token_data_identifier_t),
        [TOKEN_TYPE_KEYWORD] = sizeof(token_data_keyword_t),
        [TOKEN_TYPE_CONSTANT] = sizeof(token_data_constant_t),
        [TOKEN_TYPE_PUNCTUATION] = sizeof(token_data_punctuation_t),
        [TOKEN_TYPE_STRING_LITERAL] = sizeof(token_data_string_literal_t),
    };

    token->type = type;
    token->data = pkcc_alloc(token_sizes[type]);
}

void token_free(token_t * token) {
    if (token->type == TOKEN_TYPE_IDENTIFIER) {
        token_data_identifier_t * data = token->data;

        pkcc_free(data->name);
    }
    else if (token->type == TOKEN_TYPE_STRING_LITERAL) {
        token_data_string_literal_t * data = token->data;

        pkcc_free(data->content);
    }

    pkcc_free(token->data);
}