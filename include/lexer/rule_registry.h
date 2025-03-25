#pragma once

#include <lexer/rule.h>

#include <scanner/token.h>

typedef struct {
    size_t rule_count;
    rule_t ** rules;
} rule_registry_result_t;

void rule_registry_init(void);

rule_registry_result_t rule_registry_lookup(nonterminal_t nonterminal);
void rule_registry_result_free(rule_registry_result_t * result);