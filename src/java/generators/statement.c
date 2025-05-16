#include <parser/rules.h>

#include <java/java_default_value.h>
#include <java/generators/statement.h>
#include <java/generators/expression.h>
#include <java/generators/code_line.h>
#include <java/generators/code_list.h>
#include <java/generators/local_var.h>

#include <debug/line_error.h>

java_variable_registry_node_t * java_generator_statement(
    java_code_generator_t * jcg,
    syntax_tree_traverser_t stt,
    type_checker_type_t * function_type,
    java_label_t * break_label,
    java_label_t * continue_label
) {
    syntax_tree_traverser_t statement_stt;
    syntax_tree_traverser_enter(&stt, &statement_stt);

    if (statement_stt.current_node->token_type == RT_TERMINAL) {
        switch (statement_stt.current_node->terminal.terminal) {
            case token_number_keyword(SCANNER_KEYWORD_TYPE_BREAK): {
                if (break_label == NULL) {
                    fatal_line_full_error(
                        jcg->syntax_tree->line_buffer,
                        jcg->syntax_tree->token_buffer->tokens[statement_stt.current_node->terminal.position].file_name,
                        "Break located outside of loop",
                        jcg->syntax_tree->token_buffer->tokens[statement_stt.current_node->terminal.position].line_index
                    );
                }

                fprintf(
                    jcg->out_file,
                    "\t\tgoto %s\n",
                    break_label->name
                );
            } break;

            case token_number_keyword(SCANNER_KEYWORD_TYPE_CONTINUE): {
                if (continue_label == NULL) {
                    fatal_line_full_error(
                        jcg->syntax_tree->line_buffer,
                        jcg->syntax_tree->token_buffer->tokens[statement_stt.current_node->terminal.position].file_name,
                        "Continue located outside of loop",
                        jcg->syntax_tree->token_buffer->tokens[statement_stt.current_node->terminal.position].line_index
                    );
                }

                fprintf(
                    jcg->out_file,
                    "\t\tgoto %s\n",
                    continue_label->name
                );
            } break;

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

            case token_number_keyword(SCANNER_KEYWORD_TYPE_IF): {
                java_label_t * mid_label = java_label_manager_add(&jcg->jlm);
                java_label_t * end_label = java_label_manager_add(&jcg->jlm);

                if (syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_EXPRESSION)) {
                    java_generator_expression(jcg, statement_stt);
                }

                fprintf(
                    jcg->out_file,
                    "\t\tifeq %s\n",
                    mid_label->name
                );

                syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_CODE_LINE);
                java_generator_code_line(jcg, statement_stt, function_type, break_label, continue_label);

                fprintf(
                    jcg->out_file,
                    "\t\tgoto %s\n"
                    "\t%s:\n",
                    end_label->name,
                    mid_label->name
                );

                if (syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_STATEMENT_IF_ELSE)) {
                    syntax_tree_traverser_t else_stt;
                    syntax_tree_traverser_enter(&statement_stt, &else_stt);

                    if (syntax_tree_traverser_seek_nonterminal(&else_stt, NT_CODE_LINE)) {
                        java_generator_code_line(jcg, else_stt, function_type, break_label, continue_label);
                    }
                }

                fprintf(
                    jcg->out_file,
                    "\t%s:\n",
                    end_label->name
                );
            } break;

            case token_number_keyword(SCANNER_KEYWORD_TYPE_FOR): {
                java_label_t * start_label = java_label_manager_add(&jcg->jlm);
                java_label_t * cmp_label = java_label_manager_add(&jcg->jlm);
                java_label_t * end_label = java_label_manager_add(&jcg->jlm);
                java_label_t * cont_label = java_label_manager_add(&jcg->jlm);

                bool has_test_exp = false;
                syntax_tree_traverser_t test_exp_stt;

                bool has_update_exp = false;
                syntax_tree_traverser_t update_exp_stt;

                if (syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_STATEMENT_FOR_PARAM_1)) {
                    syntax_tree_traverser_t param1_stt;
                    syntax_tree_traverser_enter(&statement_stt, &param1_stt);

                    if (syntax_tree_traverser_seek_nonterminal(&param1_stt, NT_DECL_VAR)) {
                        java_generator_local_var(jcg, param1_stt, function_type);
                    }
                    else if (syntax_tree_traverser_seek_nonterminal(&param1_stt, NT_EXPRESSION)) {
                        java_generator_expression(jcg, param1_stt);
                        fprintf(jcg->out_file, "\t\tpop\n");
                    }
                }

                if (syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_OPTIONAL_EXPRESSION)) {
                    syntax_tree_traverser_enter(&statement_stt, &test_exp_stt);

                    syntax_tree_traverser_seek_nonterminal(&test_exp_stt, NT_EXPRESSION);
                    has_test_exp = true;
                }

                syntax_tree_traverser_next(&statement_stt);

                fprintf(
                    jcg->out_file,
                    "\t%s:\n",
                    cont_label->name
                );

                if (syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_OPTIONAL_EXPRESSION)) {
                    syntax_tree_traverser_enter(&statement_stt, &update_exp_stt);

                    syntax_tree_traverser_seek_nonterminal(&update_exp_stt, NT_EXPRESSION);
                    has_update_exp = true;
                }

                fprintf(
                    jcg->out_file,
                    "\t\tgoto %s\n"
                    "\t%s:\n",
                    cmp_label->name,
                    start_label->name
                );

                if (syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_CODE_LINE)) {
                    java_generator_code_line(jcg, statement_stt, function_type, end_label, cont_label);
                }

                fprintf(
                    jcg->out_file,
                    "\t%s:\n",
                    cmp_label->name
                );

                if (has_update_exp) {
                    java_generator_expression(jcg, update_exp_stt);
                    fprintf(
                        jcg->out_file,
                        "\t\tpop\n"
                    );
                }

                if (has_test_exp) {
                    java_generator_expression(jcg, test_exp_stt);

                    fprintf(
                        jcg->out_file,
                        "\t\tifne %s\n",
                        start_label->name
                    );
                }
                else {
                    fprintf(
                        jcg->out_file,
                        "\t\tgoto %s\n",
                        start_label->name
                    );
                }

                fprintf(
                    jcg->out_file,
                    "\t%s:\n",
                    end_label->name
                );
            } break;

            case token_number_keyword(SCANNER_KEYWORD_TYPE_WHILE): {
                java_label_t * start_label = java_label_manager_add(&jcg->jlm);
                java_label_t * cmp_label = java_label_manager_add(&jcg->jlm);
                java_label_t * end_label = java_label_manager_add(&jcg->jlm);

                syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_EXPRESSION);
                syntax_tree_traverser_t test_exp_stt = statement_stt;

                fprintf(
                    jcg->out_file,
                    "\t\tgoto %s\n"
                    "\t%s:\n",
                    cmp_label->name,
                    start_label->name
                );

                if (syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_CODE_LINE)) {
                    java_generator_code_line(jcg, statement_stt, function_type, end_label, cmp_label);
                }

                fprintf(
                    jcg->out_file,
                    "\t%s:\n",
                    cmp_label->name
                );

                java_generator_expression(jcg, test_exp_stt);

                fprintf(
                    jcg->out_file,
                    "\t\tifne %s\n"
                    "\t%s:\n",
                    start_label->name,
                    end_label->name
                );
            } break;

            case token_number_keyword(SCANNER_KEYWORD_TYPE_DO): {
                java_label_t * start_label = java_label_manager_add(&jcg->jlm);
                java_label_t * cmp_label = java_label_manager_add(&jcg->jlm);
                java_label_t * end_label = java_label_manager_add(&jcg->jlm);

                fprintf(
                    jcg->out_file,
                    "\t%s:\n",
                    start_label->name
                );

                syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_CODE_LINE);
                java_generator_code_line(jcg, statement_stt, function_type, end_label, cmp_label);

                fprintf(
                    jcg->out_file,
                    "\t%s:\n",
                    cmp_label->name
                );

                syntax_tree_traverser_seek_nonterminal(&statement_stt, NT_EXPRESSION);
                java_generator_expression(jcg,statement_stt);

                fprintf(
                    jcg->out_file,
                    "\t\tifne %s\n"
                    "\t%s:\n",
                    start_label->name,
                    end_label->name
                );
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

            case NT_CODE_BLOCK: {
                syntax_tree_traverser_t code_block_stt;
                syntax_tree_traverser_enter(&statement_stt, &code_block_stt);

                if (syntax_tree_traverser_seek_nonterminal(&code_block_stt, NT_CODE_LIST)) {
                    java_generator_code_list(jcg, code_block_stt, function_type, break_label, continue_label);
                }
            } break;
        }
    }
}