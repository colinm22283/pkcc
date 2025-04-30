#include <parser/type_checker/type_verifier.h>

#include <debug/line_error.h>
#include <debug/line_warning.h>

void type_verifier_run_recur(line_buffer_t * line_buffer, token_buffer_t * token_buffer, syntax_tree_node_t * node) {
    syntax_tree_node_list_node_t * subnode = node->head->next;

    while (subnode != node->tail) {
        if (subnode->type != NULL) {
            if (subnode->desired_type != NULL) {
                type_checker_type_convertable_result_t convertable_result = type_checker_type_convertable(subnode->desired_type, subnode->type);

                token_t * token;
                if (subnode->token_type == RT_NONTERMINAL) token = &token_buffer->tokens[subnode->nonterminal.position];
                else token = &token_buffer->tokens[subnode->terminal.position];

                if (convertable_result == TCR_NARROWING) {
                    line_warning(
                        line_buffer,
                        token->file_name,
                        "Narrowing conversion",
                        token->line_index,
                        token->position
                    );
                }
                else if (convertable_result == TCR_NOT_CONVERTABLE) {
                    char * dynamic_desired_str = type_checker_type_stringify(subnode->desired_type);
                    char * dynamic_type_str = type_checker_type_stringify(subnode->type);

                    char desired_str[strlen(dynamic_desired_str) + 1];
                    strcpy(desired_str, dynamic_desired_str);
                    pkcc_free(dynamic_desired_str);

                    char type_str[strlen(dynamic_type_str) + 1];
                    strcpy(type_str, dynamic_type_str);
                    pkcc_free(dynamic_type_str);

                    fatal_formatted_line_full_error(
                        line_buffer,
                        token_buffer->tokens[subnode->nonterminal.tree.head->next->terminal.position].file_name,
                        token_buffer->tokens[subnode->nonterminal.tree.head->next->terminal.position].line_index,
                        "Unable to implicitly convert '%s' to '%s'",
                        type_str,
                        desired_str
                    );
                }
            }
        }

        if (subnode->token_type == RT_NONTERMINAL) {
            type_verifier_run_recur(line_buffer, token_buffer, &subnode->nonterminal.tree);
        }

        subnode = subnode->next;
    }
}

void type_verifier_run(line_buffer_t * line_buffer, token_buffer_t * token_buffer, syntax_tree_t * syntax_tree) {
    type_verifier_run_recur(line_buffer, token_buffer, &syntax_tree->head);
}