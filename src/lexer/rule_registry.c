#include <lexer/rule_registry.h>

#include <lexer/rules.h>

#include <alloc.h>

void rule_registry_init(void) {

}

rule_registry_result_t rule_registry_lookup(nonterminal_t nonterminal) {
    size_t rule_count = 0;
    rule_t ** rules = pkcc_alloc(sizeof(rule_t *));

    for (size_t i = 0; i < RULE_COUNT; i++) {
        if (all_rules[i].nonterminal == nonterminal) {
            rules[rule_count++] = &all_rules[i];
            rules = pkcc_realloc(rules, (rule_count + 1) * sizeof(rule_t *));
        }
    }

    return (rule_registry_result_t) {
        .rule_count = rule_count,
        .rules = rules,
    };
}

void rule_registry_result_free(rule_registry_result_t * result) {
    pkcc_free(result->rules);
}