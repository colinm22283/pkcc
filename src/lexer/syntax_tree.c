#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

#include <scanner/token_stringify.h>

#include <lexer/syntax_tree.h>
#include <lexer/rule_registry.h>
#include <lexer/rules.h>

#include <debug/error_handler.h>
#include <debug/log.h>

#define SYNTAX_TREE_PARSE_FAIL (SIZE_MAX)
#define SYNTAX_TREE_PARSE_SUCCESS (SIZE_MAX - 1)

void syntax_tree_init(syntax_tree_t * syntax_tree) {

}

void syntax_tree_free(syntax_tree_t * syntax_tree) {

}

size_t syntax_tree_parse_recur(syntax_tree_t * syntax_tree, token_buffer_t * token_buffer, nonterminal_t nonterminal, size_t position) {
    size_t old_position = position;

    rule_registry_result_t registry_result = rule_registry_lookup(nonterminal);

    log_printf("PARSING NONTERMINAL %s\n", rules_nonterminal_name(nonterminal));

    size_t rule_count = registry_result.rule_count;
    rule_t ** rules = registry_result.rules;

    log_printf("RULE COUNT %zu\n", rule_count);

    for (size_t i = 0; i < rule_count; i++) {
        if (rules[i]->token_count == 0) {
            rule_registry_result_free(&registry_result);
            return position;
        }

        for (size_t j = 0; j < rules[i]->token_count; j++) {
            for (size_t k = 0; k < token_buffer->token_count; k++) {
                char buffer[TOKEN_STRINGIFY_BUFFER_REQUIREMENT];
                token_stringify(buffer, &token_buffer->tokens[k]);
                if (k == position) log_printf("\033[41m%s", buffer);
                else log_printf("%s", buffer);

                log_printf("    ");
            }

            if (token_buffer->token_count == position) log_printf("\033[41m$\n");
            else log_printf("$\n");

            if (rules[i]->tokens[j].type == RT_END) {
                rule_registry_result_free(&registry_result);

                if (position == token_buffer->token_count) return SYNTAX_TREE_PARSE_SUCCESS;
                else return SYNTAX_TREE_PARSE_FAIL;
            }
            else if (rules[i]->tokens[j].type == RT_TERMINAL) {
                if (token_number(&token_buffer->tokens[position]) == rules[i]->tokens[j].terminal) {
                    log_printf("TOKEN MATCH\n");

                    position++;

                    if (j == rules[i]->token_count - 1) {
                        rule_registry_result_free(&registry_result);
                        return position;
                    }
                }
                else {
                    log_printf("TOKEN MISMATCH %zu, %zu\n", token_number(&token_buffer->tokens[position]), rules[i]->tokens[j].terminal);

                    position = old_position;

                    break;
                }
            }
            else if (rules[i]->tokens[j].type == RT_NONTERMINAL) {
                size_t result = syntax_tree_parse_recur(
                    syntax_tree,
                    token_buffer,
                    rules[i]->tokens[j].nonterminal,
                    position
                );

                log_printf("RETURNING TO %s with %zu\n", rules_nonterminal_name(nonterminal), result);

                if (result == SYNTAX_TREE_PARSE_SUCCESS) return SYNTAX_TREE_PARSE_SUCCESS;
                else if (result == SYNTAX_TREE_PARSE_FAIL) break;
                else {
                    if (j == rules[i]->token_count - 1) {
                        rule_registry_result_free(&registry_result);

                        return result;
                    }
                    else position = result;
                }
            }
        }
    }

    rule_registry_result_free(&registry_result);

    log_printf("MATCH BAD\n");

    return SYNTAX_TREE_PARSE_FAIL;
}

void syntax_tree_parse(syntax_tree_t * syntax_tree, token_buffer_t * token_buffer) {
    size_t result = syntax_tree_parse_recur(syntax_tree, token_buffer, 0, 0);

    if (result == SYNTAX_TREE_PARSE_FAIL) fatal_error("Lexing error\n");
    if (result == SYNTAX_TREE_PARSE_SUCCESS) log_printf("Lexing success\n");
}