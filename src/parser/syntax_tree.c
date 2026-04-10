#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

#include <scanner/token_stringify.h>

#include <parser/nonterminals.h>

#include <parser/syntax_tree.h>

#include <debug/error_handler.h>
#include <debug/log.h>
#include <debug/ansi.h>

#include <alloc.h>

#define SYNTAX_TREE_PARSE_FAIL (SIZE_MAX)
#define SYNTAX_TREE_PARSE_SUCCESS (SIZE_MAX - 1)

void syntax_tree_init(syntax_tree_t * syntax_tree, line_buffer_t * line_buffer, token_buffer_t * token_buffer) {
    syntax_tree->line_buffer = line_buffer;
    syntax_tree->token_buffer = token_buffer;

    syntax_tree_node_init(&syntax_tree->head);
}

void syntax_tree_free(syntax_tree_t * syntax_tree) {
    syntax_tree_node_free(&syntax_tree->head);
}
//
// size_t syntax_tree_parse_recur(
//     syntax_tree_node_t * syntax_tree_node,
//     token_buffer_t * token_buffer,
//     nonterminal_t nonterminal,
//     size_t position,
//     size_t * deepest_position,
//     token_number_t * deepest_token_number,
//     nonterminal_t * deepest_failing_nonterminal
// ) {
//     size_t old_position = position;
//
//     rule_registry_result_t registry_result = rule_registry_lookup(nonterminal);
//
//     log_printf(A_BG_MAGENTA "PARSING NONTERMINAL %s\n", rules_nonterminal_name(nonterminal));
//
//     size_t rule_count = registry_result.rule_count;
//     rule_t ** rules = registry_result.rules;
//
//     log_printf("RULE COUNT %zu\n", rule_count);
//
//     for (size_t i = 0; i < rule_count; i++) {
//         if (rules[i]->token_count == 0) {
//             rule_registry_result_free(&registry_result);
//             return position;
//         }
//
//         for (size_t j = 0; j < rules[i]->token_count; j++) {
//             for (size_t k = 0; k < token_buffer->token_count; k++) {
//                 char buffer[TOKEN_STRINGIFY_BUFFER_REQUIREMENT];
//                 token_stringify(buffer, &token_buffer->tokens[k]);
//                 if (k == position) log_printf(A_BG_RED "%s", buffer);
//                 else log_printf("%s", buffer);
//
//                 log_printf("    ");
//             }
//
//             if (token_buffer->token_count == position) log_printf(A_BG_RED "$\n");
//             else log_printf("$\n");
//
//             if (rules[i]->tokens[j].type == RT_END) {
//                 rule_registry_result_free(&registry_result);
//
//                 if (position == token_buffer->token_count) return SYNTAX_TREE_PARSE_SUCCESS;
//                 else return SYNTAX_TREE_PARSE_FAIL;
//             }
//             else if (rules[i]->tokens[j].type == RT_TERMINAL) {
//                 if (token_number(&token_buffer->tokens[position]) == rules[i]->tokens[j].terminal) {
//                     log_printf("TOKEN MATCH\n");
//
//                     syntax_tree_node_list_node_t * new_node = pkcc_alloc(sizeof(syntax_tree_node_list_node_t));
//                     new_node->token_type = RT_TERMINAL;
//                     new_node->terminal.terminal = rules[i]->tokens[j].terminal;
//                     new_node->terminal.position = position;
//
//                     syntax_tree_node_list_link_back(syntax_tree_node, new_node);
//
//                     position++;
//
//                     if (j == rules[i]->token_count - 1) {
//                         rule_registry_result_free(&registry_result);
//                         return position;
//                     }
//
//                     if (position >= *deepest_position) *deepest_failing_nonterminal = NT_NULL;
//                 }
//                 else {
//                     log_printf("TOKEN MISMATCH %zu, %zu\n", token_number(&token_buffer->tokens[position]), rules[i]->tokens[j].terminal);
//
//                     if (position >= *deepest_position) {
//                         *deepest_position = position;
//                         *deepest_token_number = rules[i]->tokens[j].terminal;
//
//                         if (i == rule_count - 1) {
//                             *deepest_failing_nonterminal = nonterminal;
//                         }
//                     }
//
//                     syntax_tree_node_clear(syntax_tree_node);
//
//                     position = old_position;
//
//                     break;
//                 }
//             }
//             else if (rules[i]->tokens[j].type == RT_NONTERMINAL) {
//                 syntax_tree_node_list_node_t * new_node = pkcc_alloc(sizeof(syntax_tree_node_list_node_t));
//                 new_node->token_type = RT_NONTERMINAL;
//                 new_node->nonterminal.nonterminal = rules[i]->tokens[j].nonterminal;
//                 new_node->nonterminal.position = position;
//                 syntax_tree_node_init(&new_node->nonterminal.tree);
//
//                 size_t result = syntax_tree_parse_recur(
//                     &new_node->nonterminal.tree,
//                     token_buffer,
//                     rules[i]->tokens[j].nonterminal,
//                     position,
//                     deepest_position,
//                     deepest_token_number,
//                     deepest_failing_nonterminal
//                 );
//
//                 bool nonterminal_success = result != SYNTAX_TREE_PARSE_FAIL && result != position;
//
//                 if (nonterminal_success) {
//                     syntax_tree_node_list_link_back(syntax_tree_node, new_node);
//                 }
//                 else {
//                     syntax_tree_node_free(&new_node->nonterminal.tree);
//                     pkcc_free(new_node);
//                 }
//
//                 log_printf(A_BG_MAGENTA "RETURNING TO %s with %zu\n", rules_nonterminal_name(nonterminal), result);
//
//                 if (result == SYNTAX_TREE_PARSE_SUCCESS) {
//                     rule_registry_result_free(&registry_result);
//                     return SYNTAX_TREE_PARSE_SUCCESS;
//                 }
//                 else if (result == SYNTAX_TREE_PARSE_FAIL) break;
//                 else {
//                     if (j == rules[i]->token_count - 1) {
//                         rule_registry_result_free(&registry_result);
//
//                         return result;
//                     }
//                     else position = result;
//                 }
//             }
//
//             if (position >= token_buffer->token_count) break;
//         }
//     }
//
//     rule_registry_result_free(&registry_result);
//
//     log_printf("MATCH BAD\n");
//
//     return SYNTAX_TREE_PARSE_FAIL;
// }

// void syntax_tree_parse(syntax_tree_t * syntax_tree) {
//     size_t deepest_position = 0;
//     token_number_t deepest_token_number;
//     nonterminal_t deepest_failing_nonterminal = NT_NULL;
//
//     size_t result = syntax_tree_parse_recur(&syntax_tree->head, syntax_tree->token_buffer, 0, 0, &deepest_position, &deepest_token_number, &deepest_failing_nonterminal);
//
//     if (result == SYNTAX_TREE_PARSE_FAIL) {
//         if (deepest_failing_nonterminal == NT_NULL) fatal_error(
//             "Parser error in file %s on line %zu\n    Expected %s\n",
//             syntax_tree->token_buffer->tokens[deepest_position - 1].file_name->absolute_path,
//             syntax_tree->line_buffer->lines[syntax_tree->token_buffer->tokens[deepest_position - 1].line_index].metadata.source_line + 1,
//             token_number_stringify(deepest_token_number)
//         );
//         else fatal_error(
//             "Parser error in file %s on line %zu\n    Expected %s (%zu)\n",
//             syntax_tree->token_buffer->tokens[deepest_position - 1].file_name->absolute_path, // TODO: BUG file_name is null on first token
//             syntax_tree->line_buffer->lines[syntax_tree->token_buffer->tokens[deepest_position - 1].line_index].metadata.source_line + 1,
//             rules_nonterminal_report_name(deepest_failing_nonterminal),
//             deepest_failing_nonterminal
//         );
//     }
//     if (result == SYNTAX_TREE_PARSE_SUCCESS) log_printf("Lexing success\n");
// }

void syntax_tree_print_recur(token_buffer_t * token_buffer, syntax_tree_node_t * node, size_t indent) {
    syntax_tree_node_list_node_t * n = node->head->next;
    while (n != node->tail) {
        syntax_tree_node_list_node_t * next = n->next;

        switch (n->token_type) {
            case STNT_TERMINAL: {
                printf("   TERMINAL ");

                for (size_t i = 0; i < indent; i++) {
                    if (options.color_enable) {
                        if (i % 2 == 0) printf(A_FG_YELLOW);
                        else printf(A_FG_BLUE);
                    }

                    printf("│ ");
                }
                if (options.color_enable) printf(A_RESET);

                char token_string[TOKEN_STRINGIFY_BUFFER_REQUIREMENT];
                token_stringify(token_string, &token_buffer->tokens[n->terminal.position]);
                printf("%s (line %zu)", token_string, token_buffer->tokens[n->terminal.position].line_index + 1);

                printf("\n");
            } break;

            case STNT_NONTERMINAL: {
                printf("NONTERMINAL ");

                for (size_t i = 0; i < indent; i++) {
                    if (options.color_enable) {
                        if (i % 2 == 0) printf(A_FG_YELLOW);
                        else printf(A_FG_BLUE);
                    }

                    printf("│ ");
                }
                if (options.color_enable) printf(A_RESET);

                printf("%s ", rules_nonterminal_name(n->nonterminal.nonterminal));

                printf("\n");

                syntax_tree_print_recur(token_buffer, &n->nonterminal.tree, indent + 1);
            } break;

            default: break;
        }

        n = next;
    }
}

void syntax_tree_print(syntax_tree_t * syntax_tree) {
    syntax_tree_print_recur(syntax_tree->token_buffer, &syntax_tree->head, 0);
}

void syntax_tree_node_init(syntax_tree_node_t * node) {
    node->head = pkcc_alloc(sizeof(syntax_tree_node_list_node_t));
    node->tail = pkcc_alloc(sizeof(syntax_tree_node_list_node_t));

    node->head->next = node->tail;
    node->tail->prev = node->head;
}

void syntax_tree_node_free(syntax_tree_node_t * node) {
    syntax_tree_node_list_node_t * n = node->head->next;
    while (n != node->tail) {
        syntax_tree_node_list_node_t * next = n->next;

        if (n->token_type == STNT_NONTERMINAL) syntax_tree_node_free(&n->nonterminal.tree);
        pkcc_free(n);

        n = next;
    }

    pkcc_free(node->head);
    pkcc_free(node->tail);
}

void syntax_tree_node_clear(syntax_tree_node_t * node) {
    syntax_tree_node_list_node_t * n = node->head->next;
    while (n != node->tail) {
        syntax_tree_node_list_node_t * next = n->next;

        if (n->token_type == STNT_NONTERMINAL) syntax_tree_node_free(&n->nonterminal.tree);
        pkcc_free(n);

        n = next;
    }

    node->head->next = node->tail;
    node->tail->prev = node->head;
}

void syntax_tree_node_list_link_back(syntax_tree_node_t * node, syntax_tree_node_list_node_t * list_node) {
    list_node->prev = node->tail->prev;
    list_node->next = node->tail;

    node->tail->prev->next = list_node;
    node->tail->prev = list_node;
}

syntax_tree_node_list_node_t * syntax_tree_node_list_unlink_back(syntax_tree_node_t * node) {
     syntax_tree_node_list_node_t * ret = node->tail->prev;

     ret->prev->next = ret->next;
     ret->next->prev = ret->prev;

     return ret;
}
