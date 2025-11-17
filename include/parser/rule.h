#pragma once

#include <stddef.h>

#include <scanner/token_number.h>

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

#define DEFINE_RULE_MULTI15(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(oh dear)
#define DEFINE_RULE_MULTI14(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI15(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI13(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI14(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI12(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI13(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI11(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI12(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI10(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI11(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI9(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI10(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI8(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI9(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI7(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI8(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI6(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI7(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI5(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI6(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI4(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI5(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI3(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI4(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI2(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI3(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI1(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI2(_nonterminal, __VA_ARGS__))
#define DEFINE_RULE_MULTI0(_nonterminal, _tokens, ...) DEFINE_RULE(_nonterminal, _tokens) __VA_OPT__(, DEFINE_RULE_MULTI1(_nonterminal, __VA_ARGS__))

#define DEFINE_RULE_MULTI(_nonterminal, ...) DEFINE_RULE_MULTI0(_nonterminal, __VA_ARGS__)

#define DEFINE_NONTERMINAL_RULE_TOKEN(_nonterminal) (rule_token_t) { .type = RT_NONTERMINAL, .nonterminal = (_nonterminal) }
#define DEFINE_TERMINAL_RULE_TOKEN(_terminal) (rule_token_t) { .type = RT_TERMINAL, .terminal = (_terminal) }
#define DEFINE_END_RULE_TOKEN() (rule_token_t) { .type = RT_END, }

#define DEFINE_RULE_TOKENS(_name, ...) rule_token_t _name[] = { __VA_ARGS__ }