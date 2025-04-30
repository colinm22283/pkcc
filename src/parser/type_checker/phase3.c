#include <parser/rules.h>
#include <parser/type_checker/phase3.h>

void type_verifier_phase3_recur(
    FILE * out_file,
    line_buffer_t * line_buffer,
    token_buffer_t * token_buffer,
    syntax_tree_t * syntax_tree,
    syntax_tree_node_t * node
) {
    syntax_tree_node_list_node_t * list_node = node->head->next;

    while (list_node != node->tail) {
        if (list_node->token_type == RT_NONTERMINAL) {
            if (list_node->nonterminal.nonterminal == NT_STATEMENT) {
                if (
                    list_node->nonterminal.tree.head->next->token_type == RT_NONTERMINAL &&
                    list_node->nonterminal.tree.head->next->nonterminal.nonterminal == NT_EXPRESSION
                ) {
                    token_t * token = &token_buffer->tokens[list_node->nonterminal.tree.head->next->nonterminal.position];

                    char * type_str;
                    if (list_node->nonterminal.tree.head->next->desired_type == NULL) type_str = type_checker_type_stringify(list_node->nonterminal.tree.head->next->type);
                    else type_str = type_checker_type_stringify(list_node->nonterminal.tree.head->next->desired_type);

                    fprintf(
                        out_file,
                        "File %s Line %zu: expression has type %s\n",
                        token->file_name->absolute_path,
                        line_buffer->lines[token->line_index].metadata.source_line + 1,
                        type_str
                    );

                    pkcc_free(type_str);
                }
            }
            else type_verifier_phase3_recur(out_file, line_buffer, token_buffer, syntax_tree, &list_node->nonterminal.tree);
        }

        list_node = list_node->next;
    }
}

void type_verifier_phase3(FILE * out_file, line_buffer_t * line_buffer, token_buffer_t * token_buffer, syntax_tree_t * syntax_tree) {
    type_verifier_phase3_recur(
        out_file,
        line_buffer,
        token_buffer,
        syntax_tree,
        &syntax_tree->head
    );
}