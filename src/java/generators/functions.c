#include <parser/syntax_tree_traverser.h>
#include <parser/rules.h>

#include <java/java_type_stringify.h>
#include <java/java_default_value.h>
#include <java/label_manager.h>
#include <java/generators/functions.h>
#include <java/generators/expression.h>
#include <java/generators/code_list.h>

void java_generator_function(java_code_generator_t * jcg) {
    syntax_tree_traverser_t start_stt;
    syntax_tree_traverser_init(&start_stt, &jcg->syntax_tree->head);

    while (syntax_tree_traverser_seek_nonterminal(&start_stt, NT_DECL)) {
        syntax_tree_traverser_t decl_stt;
        syntax_tree_traverser_enter(&start_stt, &decl_stt);

        if (syntax_tree_traverser_seek_nonterminal(&decl_stt, NT_DECL_FUNC)) {
            log_printf("Got function declaration\n");

            type_checker_type_t * function_type = decl_stt.current_node->type;

            syntax_tree_traverser_t decl_func_stt;
            syntax_tree_traverser_enter(&decl_stt, &decl_func_stt);

            syntax_tree_traverser_seek_terminal(&decl_func_stt, token_number_identifier());
            token_t * identifier_token = &jcg->syntax_tree->token_buffer->tokens[decl_func_stt.current_node->terminal.position];

            if (syntax_tree_traverser_seek_nonterminal(&decl_func_stt, NT_DECL_FUNC_ARG_LIST)) {
                syntax_tree_traverser_t arg_list_stt;
                syntax_tree_traverser_enter(&decl_func_stt, &arg_list_stt);

                size_t arg_number = 0;

                while (true) {
                    syntax_tree_traverser_seek_nonterminal(&arg_list_stt, NT_TYPE);
                    type_checker_scope_t * arg_scope = arg_list_stt.current_node->scope;

                    syntax_tree_traverser_seek_terminal(&arg_list_stt, token_number_identifier());
                    token_t * arg_identifier_token = &jcg->syntax_tree->token_buffer->tokens[arg_list_stt.current_node->terminal.position];

                    java_variable_registry_push_local_number(
                        &jcg->jvr,
                        arg_identifier_token->identifier_data->name,
                        function_type->derived_type.function.args[arg_number],
                        arg_scope,
                        arg_number
                    );

                    arg_number++;

                    if (!syntax_tree_traverser_seek_nonterminal(&arg_list_stt, NT_DECL_FUNC_ARG_LIST_EXTRA)) break;

                    syntax_tree_traverser_enter_self(&arg_list_stt);
                }
            }

            log_printf("Pushing function variable \"%s\"\n", identifier_token->identifier_data->name);

            java_variable_registry_push_function(&jcg->jvr, identifier_token->identifier_data->name, function_type);

            if (syntax_tree_traverser_seek_nonterminal(&decl_func_stt, NT_CODE_BLOCK)) {
                log_printf("Rendering function content\n");

                char * func_type_str = java_type_stringify(function_type);
                fprintf(
                    jcg->out_file,
                    "\t.method public static %s : %s\n"
                    "\t.code stack 1024 locals 1024\n",
                    identifier_token->identifier_data->name,
                    func_type_str
                );
                pkcc_free(func_type_str);

                syntax_tree_traverser_t code_block_stt;
                syntax_tree_traverser_enter(&decl_func_stt, &code_block_stt);

                if (syntax_tree_traverser_seek_nonterminal(&code_block_stt, NT_CODE_LIST)) {
                    java_generator_code_list(jcg, code_block_stt, function_type, NULL, NULL);
                }

                if (
                    function_type->derived_type.function.return_type->is_base &&
                    function_type->derived_type.function.return_type->base_type.type == BTT_VOID
                ) {
                    fprintf(jcg->out_file, "\t\treturn\n");
                }

                fprintf(
                    jcg->out_file,
                    "\t\tnop\n"
                    "\t.end code\n"
                    "\t.end method\n\n"
                );
            }
        }

        if (!syntax_tree_traverser_seek_nonterminal(&start_stt, NT_START)) break;

        syntax_tree_traverser_enter_self(&start_stt);
    }
}