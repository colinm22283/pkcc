#include <parser/rules.h>

#include <java/java_default_value.h>
#include <java/generators/code_line.h>
#include <java/generators/statement.h>
#include <java/generators/expression.h>
#include <java/generators/local_var.h>

java_variable_registry_node_t * java_generator_local_var(
    java_code_generator_t * jcg,
    syntax_tree_traverser_t stt,
    type_checker_type_t * function_type
) {
    syntax_tree_traverser_t decl_var_stt;
    syntax_tree_traverser_enter(&stt, &decl_var_stt);

    syntax_tree_traverser_seek_nonterminal(&decl_var_stt, NT_TYPE);
    type_checker_scope_t * decl_scope = decl_var_stt.current_node->scope;

    if (syntax_tree_traverser_seek_nonterminal(&decl_var_stt, NT_DECL_VAR_NAME_LIST)) {
        syntax_tree_traverser_t decl_var_list_stt;
        syntax_tree_traverser_enter(&decl_var_stt, &decl_var_list_stt);

        while (true) {
            syntax_tree_traverser_seek_nonterminal(&decl_var_list_stt, NT_DECL_VAR_DECL);

            syntax_tree_traverser_t decl_var_decl_stt;
            syntax_tree_traverser_enter(&decl_var_list_stt, &decl_var_decl_stt);

            syntax_tree_traverser_seek_terminal(&decl_var_decl_stt, token_number_identifier());
            token_t * var_identifier_token = &jcg->syntax_tree->token_buffer->tokens[decl_var_decl_stt.current_node->terminal.position];

            log_printf("Added local variable with name \"%s\"\n", var_identifier_token->identifier_data->name);
            java_variable_registry_node_t * new_var = java_variable_registry_push_local(
                &jcg->jvr,
                var_identifier_token->identifier_data->name,
                decl_var_list_stt.current_node->type,
                decl_scope
            );

            if (syntax_tree_traverser_seek_nonterminal(&decl_var_decl_stt, NT_DECL_VAR_DECL_ARRAY)) {
                syntax_tree_traverser_t array_stt;
                syntax_tree_traverser_enter(&decl_var_decl_stt, &array_stt);

                if (syntax_tree_traverser_seek_nonterminal(&array_stt, NT_OPTIONAL_EXPRESSION)) {
                    syntax_tree_traverser_t exp_stt;
                    syntax_tree_traverser_enter(&array_stt, &exp_stt);

                    syntax_tree_traverser_seek_nonterminal(&exp_stt, NT_EXPRESSION);

                    java_generator_expression(jcg, exp_stt);
                }
            }

            if (syntax_tree_traverser_seek_nonterminal(&decl_var_decl_stt, NT_DECL_VAR_DECL_EQUAL)) {
                syntax_tree_traverser_t equal_stt;
                syntax_tree_traverser_enter(&decl_var_decl_stt, &equal_stt);

                syntax_tree_traverser_seek_nonterminal(&equal_stt, NT_EXPRESSION_LEVEL_14);

                java_generator_expression(jcg, equal_stt);
            }
            else {
                java_default_value(jcg, decl_var_list_stt.current_node->type);
            }

            java_variable_registry_store(jcg, new_var);

            if (!syntax_tree_traverser_seek_nonterminal(&decl_var_list_stt, NT_DECL_VAR_LIST)) break;

            syntax_tree_traverser_enter_self(&decl_var_list_stt);
        }
    }

    return NULL;
}