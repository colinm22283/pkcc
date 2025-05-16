#include <parser/rules.h>

#include <java/java_prefix.h>
#include <java/java_type_stringify.h>
#include <java/cast.h>
#include <java/generators/expression.h>

java_variable_registry_node_t * java_generator_expression(java_code_generator_t * jcg, syntax_tree_traverser_t stt) {
    syntax_tree_traverser_t sub_stt;
    syntax_tree_traverser_enter(&stt, &sub_stt);

    type_checker_type_t * from_type = stt.current_node->type;
    type_checker_type_t * to_type = stt.current_node->desired_type;
    if (to_type == NULL) to_type = stt.current_node->type;

    java_variable_registry_node_t * result = NULL;

    switch (stt.current_node->nonterminal.nonterminal) {
        case NT_EXPRESSION_LEVEL_0: {
            log_printf("Expression: Level 0\n");

            if (sub_stt.current_node->token_type == RT_TERMINAL) {
                if (sub_stt.current_node->terminal.terminal == token_number_constant()) {
                    token_t * constant_token = &jcg->syntax_tree->token_buffer->tokens[sub_stt.current_node->terminal.position];

                    switch (constant_token->constant_data->type) {
                        case SCANNER_CONSTANT_TYPE_FLOAT:
                            fprintf(jcg->out_file, "\t\tldc2_w %f\n", constant_token->constant_data->f);
                            fprintf(jcg->out_file, "\t\td2f\n");
                            break;

                        case SCANNER_CONSTANT_TYPE_DOUBLE:
                            fprintf(jcg->out_file, "\t\tldc2_w %lf\n", constant_token->constant_data->d);
                            break;

                        case SCANNER_CONSTANT_TYPE_SC:
                        case SCANNER_CONSTANT_TYPE_SS:
                        case SCANNER_CONSTANT_TYPE_SI:
                            fprintf(jcg->out_file, "\t\tldc_w %i\n", (int) constant_token->constant_data->i);
                            break;

                        case SCANNER_CONSTANT_TYPE_SL:
                        case SCANNER_CONSTANT_TYPE_SLL:
                            fprintf(jcg->out_file, "\t\tldc2_w %lli\n", (long long) constant_token->constant_data->i);
                            break;

                        case SCANNER_CONSTANT_TYPE_UC:
                        case SCANNER_CONSTANT_TYPE_US:
                        case SCANNER_CONSTANT_TYPE_UI:
                            fprintf(jcg->out_file, "\t\tldc_w %u\n", (unsigned int) constant_token->constant_data->i);
                            break;

                        case SCANNER_CONSTANT_TYPE_UL:
                        case SCANNER_CONSTANT_TYPE_ULL:
                            fprintf(jcg->out_file, "\t\tldc2_w %llu\n", (unsigned long long) constant_token->constant_data->i);
                            break;
                    }
                }
                else if (sub_stt.current_node->terminal.terminal == token_number_identifier()) {
                    token_t * identifier_token = &jcg->syntax_tree->token_buffer->tokens[sub_stt.current_node->terminal.position];

                    java_variable_registry_node_t * node = java_variable_registry_lookup(
                        &jcg->jvr,
                        identifier_token->identifier_data->name,
                        sub_stt.current_node->scope
                    );

                    if (node == NULL) fatal_error("Got null from JVR during expression parsing for var \"%s\"\n", identifier_token->identifier_data->name);

                    if (node->type != JVR_FUNCTION) java_variable_registry_load(jcg, node);

                    log_printf("Got variable\n");

                    result = node;
                }
                else if (sub_stt.current_node->terminal.terminal == token_number_string_literal()) {
                    token_t * string_token = &jcg->syntax_tree->token_buffer->tokens[sub_stt.current_node->terminal.position];

                    fprintf(
                        jcg->out_file,
                        "\t\tldc_w \"%s\"\n"
                        "\t\tinvokestatic Method lib440 java2c (Ljava/lang/String;)[C\n",
                        string_token->string_literal_data->content
                    );
                }
                else if (sub_stt.current_node->terminal.terminal == token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)) {
                    syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION);

                    java_generator_expression(jcg, sub_stt);
                }
                else fatal_error("Unimplemented constant\n");
            }
        } break;

        case NT_EXPRESSION_LEVEL_14: {
            log_printf("Expression: Level 14\n");

            if (syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_14_PRIME)) {
                syntax_tree_traverser_reset(&sub_stt);

                log_printf("Level 14 has write target\n");

                java_variable_registry_node_t * left_var = java_generator_expression(jcg, sub_stt);
                if (left_var == NULL) fatal_error("Got invalid l-value\n");

                if (syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_14_PRIME)) {
                    java_generator_expression(jcg, sub_stt);

                    syntax_tree_traverser_t prime_stt;
                    syntax_tree_traverser_enter(&sub_stt, &prime_stt);

                    token_number_t operator = prime_stt.current_node->terminal.terminal;

                    switch (operator) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT): {
                            java_variable_registry_store(jcg, left_var);

                            fprintf(jcg->out_file, "\t\tpop\n");

                            java_variable_registry_load(jcg, left_var);
                        } break;

                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_ADDITION_ASSIGNMENT): {
                            fprintf(jcg->out_file, "\t\t%cadd\n", java_prefix(left_var->var_type));

                            java_variable_registry_store(jcg, left_var);

                            java_variable_registry_load(jcg, left_var);
                        } break;

                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SUBTRACTION_ASSIGNMENT): {
                            fprintf(jcg->out_file, "\t\t%csub\n", java_prefix(left_var->var_type));

                            java_variable_registry_store(jcg, left_var);

                            java_variable_registry_load(jcg, left_var);
                        } break;

                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MULTIPLICATION_ASSIGNMENT): {
                            fprintf(jcg->out_file, "\t\t%cmul\n", java_prefix(left_var->var_type));

                            java_variable_registry_store(jcg, left_var);

                            java_variable_registry_load(jcg, left_var);
                        } break;

                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIVISION_ASSIGNMENT): {
                            fprintf(jcg->out_file, "\t\t%cdiv\n", java_prefix(left_var->var_type));

                            java_variable_registry_store(jcg, left_var);

                            java_variable_registry_load(jcg, left_var);
                        } break;

                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MODULO_ASSIGNMENT): {
                            fprintf(jcg->out_file, "\t\t%crem\n", java_prefix(left_var->var_type));

                            java_variable_registry_store(jcg, left_var);

                            java_variable_registry_load(jcg, left_var);
                        } break;

                        default: break;
                    }
                }
            }
            else {
                syntax_tree_traverser_reset(&sub_stt);

                result = java_generator_expression(jcg, sub_stt);
            }
        } break;

        case NT_EXPRESSION_LEVEL_14_PRIME: {
            syntax_tree_traverser_next(&sub_stt);

            java_variable_registry_node_t * left_var = java_generator_expression(jcg, sub_stt);

            if (syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_14_PRIME)) {
                java_generator_expression(jcg, sub_stt);

                syntax_tree_traverser_t prime_stt;
                syntax_tree_traverser_enter(&sub_stt, &prime_stt);

                token_number_t operator = prime_stt.current_node->terminal.terminal;

                switch (operator) {
                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT): {
                        java_variable_registry_store(jcg, left_var);

                        fprintf(jcg->out_file, "\t\tpop\n");

                        java_variable_registry_load(jcg, left_var);
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_ADDITION_ASSIGNMENT): {
                        fprintf(jcg->out_file, "\t\t%cadd\n", java_prefix(left_var->var_type));

                        java_variable_registry_store(jcg, left_var);

                        java_variable_registry_load(jcg, left_var);
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SUBTRACTION_ASSIGNMENT): {
                        fprintf(jcg->out_file, "\t\t%csub\n", java_prefix(left_var->var_type));

                        java_variable_registry_store(jcg, left_var);

                        java_variable_registry_load(jcg, left_var);
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MULTIPLICATION_ASSIGNMENT): {
                        fprintf(jcg->out_file, "\t\t%cmul\n", java_prefix(left_var->var_type));

                        java_variable_registry_store(jcg, left_var);

                        java_variable_registry_load(jcg, left_var);
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIVISION_ASSIGNMENT): {
                        fprintf(jcg->out_file, "\t\t%cdiv\n", java_prefix(left_var->var_type));

                        java_variable_registry_store(jcg, left_var);

                        java_variable_registry_load(jcg, left_var);
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MODULO_ASSIGNMENT): {
                        fprintf(jcg->out_file, "\t\t%crem\n", java_prefix(left_var->var_type));

                        java_variable_registry_store(jcg, left_var);

                        java_variable_registry_load(jcg, left_var);
                    } break;

                    default: break;
                }
            }
        } break;

        case NT_EXPRESSION_LEVEL_4: {
            log_printf("Expression: Level 4\n");

            java_variable_registry_node_t * node = java_generator_expression(jcg, sub_stt);

            if (syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_4_PRIME)) {
                java_generator_expression(jcg, sub_stt);
            }

            result = node;
        } break;

        case NT_EXPRESSION_LEVEL_4_PRIME: {
            token_number_t operator = sub_stt.current_node->terminal.terminal;

            syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_3);
            java_generator_expression(jcg, sub_stt);

            type_checker_type_t * desired_type = stt.current_node->desired_type;
            if (desired_type == NULL) desired_type = stt.current_node->type;

            switch (operator) {
                case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PLUS):
                    fprintf(jcg->out_file, "\t\t%cadd\n", java_prefix(desired_type));
                    break;

                case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MINUS):
                    fprintf(jcg->out_file, "\t\t%csub\n", java_prefix(desired_type));
                    break;

                default: break;
            }

            if (syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_4_PRIME)) java_generator_expression(jcg, sub_stt);
        } break;

        case NT_EXPRESSION_LEVEL_3: {
            log_printf("Expression: Level 3\n");

            java_variable_registry_node_t * node = java_generator_expression(jcg, sub_stt);

            if (syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_3_PRIME)) {
                java_generator_expression(jcg, sub_stt);
            }

            result = node;
        } break;

        case NT_EXPRESSION_LEVEL_3_PRIME: {
            token_number_t operator = sub_stt.current_node->terminal.terminal;

            syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_2);
            java_generator_expression(jcg, sub_stt);

            type_checker_type_t * desired_type = stt.current_node->desired_type;
            if (desired_type == NULL) desired_type = stt.current_node->type;

            switch (operator) {
                case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_STAR):
                    fprintf(jcg->out_file, "\t\t%cmul\n", java_prefix(desired_type));
                    break;

                case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SLASH):
                    fprintf(jcg->out_file, "\t\t%cdiv\n", java_prefix(desired_type));
                    break;

                case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PERCENT):
                    fprintf(jcg->out_file, "\t\t%crem\n", java_prefix(desired_type));
                    break;

                default: break;
            }

            if (syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_3_PRIME)) java_generator_expression(jcg, sub_stt);
        } break;

        case NT_EXPRESSION_LEVEL_2: {
            log_printf("Expression: Level 2\n");

            if (sub_stt.current_node->token_type == RT_TERMINAL) {
                switch (sub_stt.current_node->terminal.terminal) {
                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN): {
                        syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_TYPE);

                        type_checker_type_t * target_type = sub_stt.current_node->type;

                        syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_2);
                        type_checker_type_t * source_type = sub_stt.current_node->type;

                        java_generator_expression(jcg, sub_stt);

                        if (!java_cast(jcg, target_type, source_type)) fatal_error("Got unable to perform cast during generation\n");
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MINUS): {
                        syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_2);
                        java_generator_expression(jcg, sub_stt);

                        fprintf(jcg->out_file, "\t\t%cneg\n", java_prefix(sub_stt.current_node->type));
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_INCREMENT): {
                        syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_2);
                        java_variable_registry_node_t * lval = java_generator_expression(jcg, sub_stt);

                        if (lval == NULL) fatal_error("Bad lvalue for decrement\n");

                        fprintf(
                            jcg->out_file,
                            "\t\t%cconst_1\n"
                            "\t\t%cadd\n"
                            "\t\tdup\n",
                            java_prefix(sub_stt.current_node->type),
                            java_prefix(sub_stt.current_node->type)
                        );
                        java_variable_registry_store(jcg, lval);

                        result = lval;
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DECREMENT): {
                        syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_2);
                        java_variable_registry_node_t * lval = java_generator_expression(jcg, sub_stt);

                        if (lval == NULL) fatal_error("Bad lvalue for decrement\n");

                        fprintf(
                            jcg->out_file,
                            "\t\t%cconst_1\n"
                            "\t\t%csub\n"
                            "\t\tdup\n",
                            java_prefix(sub_stt.current_node->type),
                            java_prefix(sub_stt.current_node->type)
                        );
                        java_variable_registry_store(jcg, lval);

                        result = lval;
                    } break;

                    default: break;
                }
            }
            else {
                result = java_generator_expression(jcg, sub_stt);
            }
        } break;

        case NT_EXPRESSION_LEVEL_1: {
            log_printf("Expression: Level 1\n");

            type_checker_type_t * left_type = sub_stt.current_node->type;
            java_variable_registry_node_t * left_var = java_generator_expression(jcg, sub_stt);

            result = left_var;

            if (syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_1_PRIME)) {
                syntax_tree_traverser_t inner_stt;
                syntax_tree_traverser_enter(&sub_stt, &inner_stt);

                token_number_t operator = inner_stt.current_node->terminal.terminal;

                switch (operator) {
                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN): {
                        if (syntax_tree_traverser_seek_nonterminal(&inner_stt, NT_CALL_ARG_LIST)) {
                            syntax_tree_traverser_t arg_list_stt;
                            syntax_tree_traverser_enter(&inner_stt, &arg_list_stt);

                            while (true) {
                                syntax_tree_traverser_seek_nonterminal(&arg_list_stt, NT_EXPRESSION_LEVEL_14);
                                java_generator_expression(jcg, arg_list_stt);

                                if (!syntax_tree_traverser_seek_nonterminal(&arg_list_stt, NT_CALL_ARG_LIST_EXTRA)) break;

                                syntax_tree_traverser_enter_self(&arg_list_stt);
                            }
                        }

                        java_variable_registry_call(
                            jcg->out_file,
                            jcg->classname,
                            left_var
                        );
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SQUARE_OPEN): {
                        syntax_tree_traverser_seek_nonterminal(&inner_stt, NT_EXPRESSION);

                        java_generator_expression(jcg, inner_stt);

                        fprintf(
                            jcg->out_file,
                            "\t\t%caload\n",
                            java_prefix(left_type->derived_type.array.subtype)
                        );

                        java_variable_registry_node_t * array_ref = java_variable_registry_push_array_ref(
                            &jcg->jvr,
                            left_var,
                            inner_stt
                        );

                        result = array_ref;
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_INCREMENT): {
                        fprintf(
                            jcg->out_file,
                            "\t\tdup\n"
                            "\t\t%cconst_1\n"
                            "\t\t%cadd\n",
                            java_prefix(left_type),
                            java_prefix(left_type)
                        );
                        java_variable_registry_store(jcg, left_var);

                        result = left_var;
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DECREMENT): {
                        fprintf(
                            jcg->out_file,
                            "\t\tdup\n"
                            "\t\t%cconst_1\n"
                            "\t\t%csub\n",
                            java_prefix(left_type),
                            java_prefix(left_type)
                        );
                        java_variable_registry_store(jcg, left_var);

                        result = left_var;
                    } break;

                    default: break;
                }
            }
        } break;

        case NT_EXPRESSION_LEVEL_1_PRIME: {
        } break;

        default: {
            result = java_generator_expression(jcg, sub_stt);
        } break;
    }

    java_cast(jcg, to_type, from_type);

    return result;
}