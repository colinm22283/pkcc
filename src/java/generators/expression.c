#include <parser/rules.h>

#include <java/java_prefix.h>
#include <java/java_type_stringify.h>
#include <java/generators/expression.h>

java_variable_registry_node_t * java_generator_expression(java_code_generator_t * jcg, syntax_tree_traverser_t stt) {
    syntax_tree_traverser_t sub_stt;
    syntax_tree_traverser_enter(&stt, &sub_stt);

    switch (stt.current_node->nonterminal.nonterminal) {
        case NT_EXPRESSION_LEVEL_0: {
            log_printf("Expression: Level 0\n");

            if (sub_stt.current_node->token_type == RT_TERMINAL) {
                if (sub_stt.current_node->terminal.terminal == token_number_constant()) {
                    token_t * constant_token = &jcg->syntax_tree->token_buffer->tokens[sub_stt.current_node->terminal.position];

                    switch (constant_token->constant_data->type) {
                        case SCANNER_CONSTANT_TYPE_FLOAT:
                            fprintf(jcg->out_file, "\t\tldc_w %f\n", constant_token->constant_data->f);
                            break;

                        case SCANNER_CONSTANT_TYPE_DOUBLE:
                            fprintf(jcg->out_file, "\t\tldc_w %lf\n", constant_token->constant_data->d);
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

                    return NULL;
                }
                else if (sub_stt.current_node->terminal.terminal == token_number_identifier()) {
                    token_t * identifier_token = &jcg->syntax_tree->token_buffer->tokens[sub_stt.current_node->terminal.position];

                    java_variable_registry_node_t * node = java_variable_registry_lookup(
                        &jcg->jvr,
                        identifier_token->identifier_data->name,
                        sub_stt.current_node->scope
                    );

                    if (node == NULL) fatal_error("Got null from JVR during global init\n");

                    java_variable_registry_load(jcg->out_file, jcg->classname, node);

                    log_printf("Got variable\n");

                    return node;
                }
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
                            java_variable_registry_store(jcg->out_file, jcg->classname, left_var);

                            fprintf(jcg->out_file, "\t\tpop\n");

                            java_variable_registry_load(jcg->out_file, jcg->classname, left_var);
                        } break;

                        default: break;
                    }
                }
            }
            else {
                syntax_tree_traverser_reset(&sub_stt);

                return java_generator_expression(jcg, sub_stt);
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
                        java_variable_registry_store(jcg->out_file, jcg->classname, left_var);

                        fprintf(jcg->out_file, "\t\tpop\n");

                        java_variable_registry_load(jcg->out_file, jcg->classname, left_var);
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

            return node;
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

            return node;
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
                    fprintf(jcg->out_file, "\t\t%cmod\n", java_prefix(desired_type));
                    break;

                default: break;
            }

            if (syntax_tree_traverser_seek_nonterminal(&sub_stt, NT_EXPRESSION_LEVEL_3_PRIME)) java_generator_expression(jcg, sub_stt);
        } break;

        default: {
            return java_generator_expression(jcg, sub_stt);
        } break;
    }

    return NULL;
}