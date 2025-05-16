#include <parser/syntax_tree_traverser.h>
#include <parser/rules.h>

#include <java/java_type_stringify.h>
#include <java/java_prefix.h>
#include <java/java_default_value.h>
#include <java/generators/functions.h>
#include <java/generators/expression.h>

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
                    syntax_tree_traverser_t code_list_stt;
                    syntax_tree_traverser_enter(&code_block_stt, &code_list_stt);

                    while (true) {
                        syntax_tree_traverser_seek_nonterminal(&code_list_stt, NT_CODE_LINE);

                        syntax_tree_traverser_t code_line_stt;
                        syntax_tree_traverser_enter(&code_list_stt, &code_line_stt);

                        if (syntax_tree_traverser_seek_nonterminal(&code_line_stt, NT_STATEMENT)) {
                            syntax_tree_traverser_t statement_stt;
                            syntax_tree_traverser_enter(&code_line_stt, &statement_stt);

                            if (statement_stt.current_node->token_type == RT_TERMINAL) {
                                switch (statement_stt.current_node->terminal.terminal) {
                                    case token_number_keyword(SCANNER_KEYWORD_TYPE_RETURN): {
                                        log_printf("Got return in function\n");

                                        if (syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_STATEMENT_RETURN_VALUE)) {

                                            syntax_tree_traverser_t return_value_stt;
                                            syntax_tree_traverser_enter(&statement_stt, &return_value_stt);

                                            syntax_tree_traverser_seek_nonterminal(&return_value_stt, NT_EXPRESSION);

                                            java_generator_expression(jcg, return_value_stt);

                                            switch (function_type->derived_type.function.return_type->base_type.type) {
                                                case BTT_VOID:
                                                    fprintf(jcg->out_file, "\t\treturn\n");
                                                    break;

                                                case BTT_BOOL:
                                                case BTT_CHAR:
                                                case BTT_SHORT:
                                                case BTT_INT:
                                                    fprintf(jcg->out_file, "\t\tireturn\n");
                                                    break;

                                                case BTT_LONG:
                                                case BTT_LONG_LONG:
                                                    fprintf(jcg->out_file, "\t\tlreturn\n");
                                                    break;

                                                case BTT_FLOAT:
                                                    fprintf(jcg->out_file, "\t\tfreturn\n");
                                                    break;

                                                case BTT_DOUBLE:
                                                    fprintf(jcg->out_file, "\t\tdreturn\n");
                                                    break;
                                            }
                                        }
                                        else fprintf(jcg->out_file, "\t\treturn\n");
                                    } break;
                                }
                            }
                            else {
                                switch (statement_stt.current_node->nonterminal.nonterminal) {
                                    case NT_EXPRESSION: {
                                        syntax_tree_traverser_t expression_stt;
                                        syntax_tree_traverser_enter(&statement_stt, &expression_stt);

                                        type_checker_type_t * expression_type = expression_stt.current_node->type;

                                        java_generator_expression(jcg, expression_stt);

                                        if (expression_type->is_base && expression_type->base_type.type == BTT_VOID) {
                                            break;
                                        }

                                        fprintf(jcg->out_file, "\t\tpop\n");
                                    } break;
                                }
                            }
                        }
                        else if (syntax_tree_traverser_seek_nonterminal(&code_line_stt, NT_DECL_VAR)) {
                            syntax_tree_traverser_t decl_var_stt;
                            syntax_tree_traverser_enter(&code_line_stt, &decl_var_stt);

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
                        }

                        if (!syntax_tree_traverser_seek_nonterminal(&code_list_stt, NT_CODE_LIST)) break;

                        syntax_tree_traverser_enter_self(&code_list_stt);
                    }
                }

                if (
                    function_type->derived_type.function.return_type->is_base &&
                    function_type->derived_type.function.return_type->base_type.type == BTT_VOID
                ) {
                    fprintf(jcg->out_file, "\t\treturn\n");
                }

                fprintf(
                    jcg->out_file,
                    "\t.end code\n"
                    "\t.end method\n\n"
                );
            }
        }

        if (!syntax_tree_traverser_seek_nonterminal(&start_stt, NT_START)) break;

        syntax_tree_traverser_enter_self(&start_stt);
    }
}