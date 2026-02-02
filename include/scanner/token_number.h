#pragma once

#include <scanner/token.h>
#include <scanner/phase1.h>

enum {
    TOKEN_NUMBER_IDENTIFIER_START     = 0x00000000,
    TOKEN_NUMBER_KEYWORD_START        = 0x10000000,
    TOKEN_NUMBER_CONSTANT_START       = 0x20000000,
    TOKEN_NUMBER_PUNCTUATION_START    = 0x30000000,
    TOKEN_NUMBER_STRING_LITERAL_START = 0x40000000,
    TOKEN_NUMBER_END_START            = 0x50000000,
};

typedef size_t token_number_t;

static inline token_number_t token_number(token_t * token) {
    switch (token->type) {
        case TOKEN_TYPE_IDENTIFIER: return TOKEN_NUMBER_IDENTIFIER_START;
        case TOKEN_TYPE_KEYWORD: return TOKEN_NUMBER_KEYWORD_START + ((token_data_keyword_t *) token->data)->keyword;
        case TOKEN_TYPE_CONSTANT: return TOKEN_NUMBER_CONSTANT_START;
        case TOKEN_TYPE_PUNCTUATION: return TOKEN_NUMBER_PUNCTUATION_START + ((token_data_punctuation_t *) token->data)->type;
        case TOKEN_TYPE_STRING_LITERAL: return TOKEN_NUMBER_STRING_LITERAL_START;

        case TOKEN_TYPE_END: return TOKEN_NUMBER_END_START;

        default: fatal_error("INVALID INPUT to token_number()\n");
    }
}

static inline const char * token_number_stringify(token_number_t token_number) {
    token_t token;

    if (token_number < TOKEN_NUMBER_KEYWORD_START) {
        return "identifier";
    }
    else if (token_number < TOKEN_NUMBER_CONSTANT_START) {
        token_data_keyword_t keyword;
        token.type = TOKEN_TYPE_KEYWORD;
        token.data = &keyword;
        keyword.keyword = token_number - TOKEN_NUMBER_KEYWORD_START;

        return scanner_stringify_phase1(&token);
    }
    else if (token_number < TOKEN_NUMBER_PUNCTUATION_START) {
        return "constant";
    }
    else if (token_number < TOKEN_NUMBER_STRING_LITERAL_START) {
        token_data_punctuation_t punctuation;
        token.type = TOKEN_TYPE_PUNCTUATION;
        token.data = &punctuation;
        punctuation.type = token_number - TOKEN_NUMBER_PUNCTUATION_START;

        return scanner_stringify_phase1(&token);
    }
    else if (token_number < TOKEN_NUMBER_END_START) {
        return "string literal";
    }
    else {
        return "end";
    }
}

#define token_number_identifier() (TOKEN_NUMBER_IDENTIFIER_START)
#define token_number_keyword(keyword) (TOKEN_NUMBER_KEYWORD_START + (keyword))
#define token_number_constant() (TOKEN_NUMBER_CONSTANT_START)
#define token_number_punctuation(punctuation) (TOKEN_NUMBER_PUNCTUATION_START + (punctuation))
#define token_number_string_literal() (TOKEN_NUMBER_STRING_LITERAL_START)
#define token_number_end() (TOKEN_NUMBER_END_START)
