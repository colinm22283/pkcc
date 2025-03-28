#include <stdbool.h>

#include <parser/phase2.h>
#include <parser/rules.h>

typedef enum {
    P2S_GLOBAL,
    P2S_STRUCT_DECL,
    P2S_FUNCTION_DECL,
    P2S_FUNCTION_DECL_ARG_LIST,
    P2S_NAMED_VAR,
    P2S_CODE_BLOCK,
} phase2_state_t;

void parser_print_phase2_recur(FILE * out_file, syntax_tree_t * syntax_tree, syntax_tree_node_t * node, phase2_state_t state) {
    syntax_tree_node_list_node_t * n = node->head->next;

    while (n != node->tail) {
        if (n->token_type == RT_NONTERMINAL) {
            switch (n->nonterminal.nonterminal) {
                case NT_DECL_FUNC: {
                    token_t * token = &syntax_tree->token_buffer->tokens[n->nonterminal.position];

                    token_t * name_token = &syntax_tree->token_buffer->tokens[n->nonterminal.tree.head->next->next->terminal.position];
                    token_data_identifier_t * identifier = name_token->data;

                    fprintf(out_file, "File %s Line %zu: function %s\n", token->file_name->absolute_path, token->line + 1, identifier->name);

                    parser_print_phase2_recur(out_file, syntax_tree, &n->nonterminal.tree, P2S_FUNCTION_DECL);
                } break;

                case NT_STRUCT: {
                    token_t * token = &syntax_tree->token_buffer->tokens[n->nonterminal.position];

                    const char * name = "";
                    if (n->nonterminal.tree.head->next->next->token_type == RT_NONTERMINAL) {
                        token_t * name_token = &syntax_tree->token_buffer->tokens[n->nonterminal.tree.head->next->next->nonterminal.tree.head->next->terminal.position];
                        token_data_identifier_t * identifier = name_token->data;
                        name = identifier->name;
                    }

                    if (state == P2S_GLOBAL) fprintf(out_file, "File %s Line %zu: global struct %s\n", token->file_name->absolute_path, token->line + 1, name);
                    else if (state == P2S_CODE_BLOCK) fprintf(out_file, "File %s Line %zu: local struct %s\n", token->file_name->absolute_path, token->line + 1, name);

                    parser_print_phase2_recur(out_file, syntax_tree, &n->nonterminal.tree, P2S_STRUCT_DECL);
                } break;

                case NT_DECL_VAR: {
                    token_t * token = &syntax_tree->token_buffer->tokens[n->nonterminal.position];

                    bool has_name = false;

                    if (state == P2S_CODE_BLOCK) {
                        syntax_tree_node_list_node_t * temp = n->nonterminal.tree.head->next;
                        while (temp != n->nonterminal.tree.tail) {
                            if (temp->token_type == RT_NONTERMINAL && temp->nonterminal.nonterminal == NT_DECL_VAR_NAME_LIST) {
                                has_name = true;

                                break;
                            }

                            temp = temp->next;
                        }
                    }

                    if (has_name) parser_print_phase2_recur(out_file, syntax_tree, &n->nonterminal.tree, P2S_NAMED_VAR);
                    else parser_print_phase2_recur(out_file, syntax_tree, &n->nonterminal.tree, state);
                } break;

                case NT_DECL_VAR_DECL: {
                    token_t * token = &syntax_tree->token_buffer->tokens[n->nonterminal.position];

                    token_t * name_token = &syntax_tree->token_buffer->tokens[n->nonterminal.tree.head->next->terminal.position];
                    token_data_identifier_t * identifier = name_token->data;

                    if (state == P2S_GLOBAL) fprintf(out_file, "File %s Line %zu: global variable %s\n", token->file_name->absolute_path, token->line + 1, identifier->name);
                    else if (state == P2S_STRUCT_DECL) fprintf(out_file, "File %s Line %zu: member %s\n", token->file_name->absolute_path, token->line + 1, identifier->name);
                    else if (state == P2S_FUNCTION_DECL) fprintf(out_file, "File %s Line %zu: parameter %s\n", token->file_name->absolute_path, token->line + 1, identifier->name);
                    else if (state == P2S_CODE_BLOCK || state == P2S_NAMED_VAR) {
                        fprintf(out_file, "File %s Line %zu: local variable %s\n", token->file_name->absolute_path, token->line + 1, identifier->name);
                    }

                    parser_print_phase2_recur(out_file, syntax_tree, &n->nonterminal.tree, state);
                } break;

                case NT_DECL_FUNC_ARG_LIST: {
                    token_t * token = &syntax_tree->token_buffer->tokens[n->nonterminal.position];

                    token_t * name_token = &syntax_tree->token_buffer->tokens[n->nonterminal.tree.head->next->next->terminal.position];
                    token_data_identifier_t * identifier = name_token->data;

                    if (state == P2S_FUNCTION_DECL) fprintf(out_file, "File %s Line %zu: parameter %s\n", token->file_name->absolute_path, token->line + 1, identifier->name);

                    parser_print_phase2_recur(out_file, syntax_tree, &n->nonterminal.tree, P2S_FUNCTION_DECL_ARG_LIST);
                } break;

                case NT_DECL_FUNC_ARG_LIST_EXTRA: {
                    token_t * token = &syntax_tree->token_buffer->tokens[n->nonterminal.position];

                    token_t * name_token = &syntax_tree->token_buffer->tokens[n->nonterminal.tree.head->next->next->next->terminal.position];
                    token_data_identifier_t * identifier = name_token->data;

                    if (state == P2S_FUNCTION_DECL_ARG_LIST) fprintf(out_file, "File %s Line %zu: parameter %s\n", token->file_name->absolute_path, token->line + 1, identifier->name);

                    parser_print_phase2_recur(out_file, syntax_tree, &n->nonterminal.tree, state);
                } break;

                case NT_CODE_BLOCK: {
                    parser_print_phase2_recur(out_file, syntax_tree, &n->nonterminal.tree, P2S_CODE_BLOCK);
                } break;

                default: {
                    parser_print_phase2_recur(out_file, syntax_tree, &n->nonterminal.tree, state);
                } break;
            }
        }

        n = n->next;
    }
}

void parser_print_phase2(FILE * out_file, parser_t * parser) {
    parser_print_phase2_recur(out_file, &parser->syntax_tree, &parser->syntax_tree.head, P2S_GLOBAL);
}