#pragma once

#include <stddef.h>

#include <scanner/token.h>

typedef enum {
    RT_TERMINAL,
    RT_NONTERMINAL,
    RT_END,
} rule_token_type_t;

typedef size_t nonterminal_t;

typedef struct {
    rule_token_type_t type;
    union {
        nonterminal_t nonterminal;
        token_number_t terminal;
    };
} rule_token_t;

typedef struct {
    nonterminal_t nonterminal;

    size_t token_count;
    rule_token_t * tokens;
} rule_t;

#define DEFINE_RULE(_nonterminal, _tokens) (rule_t) { .nonterminal = (_nonterminal), .token_count = sizeof(_tokens) / sizeof(rule_token_t), .tokens = (_tokens), }

#define DEFINE_NONTERMINAL_RULE_TOKEN(_nonterminal) (rule_token_t) { .type = RT_NONTERMINAL, .nonterminal = (_nonterminal) }
#define DEFINE_TERMINAL_RULE_TOKEN(_terminal) (rule_token_t) { .type = RT_TERMINAL, .terminal = (_terminal) }
#define DEFINE_END_RULE_TOKEN() (rule_token_t) { .type = RT_END, }