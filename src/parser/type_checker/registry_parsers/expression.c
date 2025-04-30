#include <parser/type_checker/error.h>
#include <parser/type_checker/registry_parsers/expression.h>

#include <debug/log.h>
#include <debug/line_error.h>

type_checker_type_t * expression_level_1_parse(
    type_checker_variable_registry_t * vr,
    line_buffer_t * line_buffer,
    token_buffer_t * token_buffer,
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_1_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_INCREMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DECREMENT):  {
                            temp->type = base_node->type;
                        } break;

                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN):  {
                            if (
                                base_node->type->is_base ||
                                base_node->type->derived_type.type != DTT_FUNCTION
                            ) {
                                fatal_line_full_error(
                                    line_buffer,
                                    token_buffer->tokens[base_node->nonterminal.position].file_name,
                                    "Call attempted on invalid value",
                                    token_buffer->tokens[base_node->nonterminal.position].line_index
                                );
                            }

                            type_checker_type_t * function_type = base_node->type;

                            syntax_tree_node_t * temp_tree = &subnode->next->nonterminal.tree;

                            for (size_t i = 0; i < function_type->derived_type.function.arg_count; i++) {
                                syntax_tree_node_list_node_t * expression_node;
                                if (temp_tree->head->next->token_type == RT_TERMINAL) expression_node = temp_tree->head->next->next;
                                else expression_node = temp_tree->head->next;

                                expression_node->desired_type = function_type->derived_type.function.args[i];

                                if (
                                    temp_tree->tail->prev->token_type == RT_NONTERMINAL &&
                                    temp_tree->tail->prev->nonterminal.nonterminal == NT_CALL_ARG_LIST_EXTRA
                                ) {
                                    temp_tree = &temp_tree->tail->prev->nonterminal.tree;
                                }
                                else {

                                }
                            }

                            temp->type = function_type->derived_type.function.return_type;
                        } break;

                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SQUARE_OPEN):  {
                            if (
                                base_node->type->is_base ||
                                base_node->type->derived_type.type != DTT_ARRAY
                            ) {
                                fatal_line_full_error(
                                    line_buffer,
                                    token_buffer->tokens[base_node->nonterminal.position].file_name,
                                    "Subscript attempted on invalid value",
                                    token_buffer->tokens[base_node->nonterminal.position].line_index
                                );
                            }

                            temp->type = base_node->type->derived_type.array.subtype;
                            subnode->next->desired_type = ulonglong_type;
                        } break;

                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PERIOD): {
                            type_checker_type_t * struct_type = base_node->type;
                            if (
                                struct_type->is_base ||
                                struct_type->derived_type.type != DTT_STRUCT
                            ) {
                                fatal_line_full_error(
                                    line_buffer,
                                    token_buffer->tokens[base_node->nonterminal.position].file_name,
                                    "Member access attempted on invalid value",
                                    token_buffer->tokens[base_node->nonterminal.position].line_index
                                );
                            }

                            log_printf("TEST: %s\n", type_checker_type_stringify(struct_type));

                            const char * member_name = token_buffer->tokens[subnode->next->terminal.position].identifier_data->name;

                            bool found = false;
                            size_t location;
                            log_printf("TEST: %zu\n", struct_type->derived_type.structure.subtype_count);
                            for (location = 0; location < struct_type->derived_type.structure.subtype_count; location++) {
                                if (struct_type->derived_type.structure.subtype_names[location] != NULL) {
                                    log_printf("TEST: %s\n", struct_type->derived_type.structure.subtype_names[location]);

                                    if (strcmp(member_name, struct_type->derived_type.structure.subtype_names[location]) == 0) {
                                        found = true;
                                        break;
                                    }
                                }
                            }

                            if (!found) {
                                fatal_formatted_line_full_error(
                                    line_buffer,
                                    token_buffer->tokens[base_node->nonterminal.position].file_name,
                                    token_buffer->tokens[base_node->nonterminal.position].line_index,
                                    "Member '%s' not found in struct",
                                    member_name
                                );
                            }

                            temp->type = struct_type->derived_type.structure.subtypes[location];
                        } break;

                        default: break;
                    }

                    return expression_level_1_parse(vr, line_buffer, token_buffer, temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_3_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_3_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_STAR):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SLASH):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PERCENT): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                base_node->type,
                                subnode->next->type
                            );

                            base_node->desired_type = expanded_type;
                            temp->type = expanded_type;
                            subnode->next->desired_type = expanded_type;
                        } break;

                        default: break;
                    }

                    return expression_level_3_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_4_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_4_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PLUS):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MINUS): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                base_node->type,
                                subnode->next->type
                            );

                            base_node->desired_type = expanded_type;
                            temp->type = expanded_type;
                            subnode->next->desired_type = expanded_type;
                        } break;

                        default: break;
                    }

                    return expression_level_4_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_5_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_5_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                base_node->type,
                                subnode->next->type
                            );

                            base_node->desired_type = expanded_type;
                            temp->type = expanded_type;
                            subnode->next->desired_type = expanded_type;
                        } break;

                        default: break;
                    }

                    return expression_level_5_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_6_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_6_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LESS_THAN):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LESS_THAN_OR_EQUAL_TO):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_GREATER_THAN):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_GREATER_THAN_OR_EQUAL_TO): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                base_node->type,
                                subnode->next->type
                            );

                            base_node->desired_type = expanded_type;
                            subnode->next->desired_type = expanded_type;

                            temp->type = bool_type;
                        } break;

                        default: break;
                    }

                    return expression_level_6_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_7_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_7_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_EQUAL_TO):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_NOT_EQUAL_TO): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                base_node->type,
                                subnode->next->type
                            );

                            base_node->desired_type = expanded_type;
                            subnode->next->desired_type = expanded_type;

                            temp->type = bool_type;
                        } break;

                        default: break;
                    }

                    return expression_level_7_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_8_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_8_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_AND): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                base_node->type,
                                subnode->next->type
                            );

                            base_node->desired_type = expanded_type;
                            temp->type = expanded_type;
                            subnode->next->desired_type = expanded_type;
                        } break;

                        default: break;
                    }

                    return expression_level_8_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_9_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_9_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_XOR): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                base_node->type,
                                subnode->next->type
                            );

                            base_node->desired_type = expanded_type;
                            temp->type = expanded_type;
                            subnode->next->desired_type = expanded_type;
                        } break;

                        default: break;
                    }

                    return expression_level_9_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_10_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_10_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_OR): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                base_node->type,
                                subnode->next->type
                            );

                            base_node->desired_type = expanded_type;
                            temp->type = expanded_type;
                            subnode->next->desired_type = expanded_type;
                        } break;

                        default: break;
                    }

                    return expression_level_10_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_11_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_11_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LOGICAL_AND): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                base_node->type,
                                subnode->next->type
                            );

                            base_node->desired_type = expanded_type;
                            subnode->next->desired_type = expanded_type;

                            temp->type = bool_type;
                        } break;

                        default: break;
                    }

                    return expression_level_11_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_12_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_12_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LOGICAL_OR): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                base_node->type,
                                subnode->next->type
                            );

                            base_node->desired_type = expanded_type;
                            subnode->next->desired_type = expanded_type;

                            temp->type = bool_type;
                        } break;

                        default: break;
                    }

                    return expression_level_12_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_13_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_13_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_QUESTION_MARK): {
                            type_checker_type_t * expanded_type = type_checker_type_expand(
                                subnode->next->type,
                                subnode->next->next->next->type
                            );

                            base_node->desired_type = bool_type;
                            subnode->next->desired_type = expanded_type;
                            subnode->next->next->next->desired_type = expanded_type;

                            temp->type = expanded_type;
                        } break;

                        default: break;
                    }

                    return expression_level_13_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_14_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_14_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_ADDITION_ASSIGNMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SUBTRACTION_ASSIGNMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MULTIPLICATION_ASSIGNMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIVISION_ASSIGNMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MODULO_ASSIGNMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT_ASSIGNMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT_ASSIGNMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_AND_ASSIGNMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_XOR_ASSIGNMENT):
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_OR_ASSIGNMENT): {
                            temp->type = base_node->type;
                            subnode->next->desired_type = base_node->type;
                        } break;

                        default: break;
                    }

                    return expression_level_14_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_level_15_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    syntax_tree_node_list_node_t * base_node
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_15_PRIME) {
                    syntax_tree_node_list_node_t * subnode = temp->nonterminal.tree.head->next;

                    switch (subnode->terminal.terminal) {
                        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA): {
                            temp->type = subnode->next->type;
                        } break;

                        default: break;
                    }

                    return expression_level_15_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * expression_top_level_parse(
    syntax_tree_node_list_node_t * node
) {
    if (node->token_type == RT_TERMINAL) return node->next->type;
    else return node->type;
}

type_checker_type_t * type_checker_registry_parse_expression(
    __MAYBE_UNUSED type_checker_registry_t * tr,
    type_checker_variable_registry_t * vr,
    __MAYBE_UNUSED line_buffer_t * line_buffer,
    __MAYBE_UNUSED token_buffer_t * token_buffer,
    __MAYBE_UNUSED syntax_tree_node_list_node_t * expression_node
) {
    if (expression_node->token_type != RT_NONTERMINAL) type_checker_error();
    if (!nonterminal_is_expression(expression_node->nonterminal.nonterminal)) type_checker_error();

    syntax_tree_node_list_node_t * subnode = expression_node->nonterminal.tree.head->next;

    switch (expression_node->nonterminal.nonterminal) {
        case NT_EXPRESSION_LEVEL_0: {
            if (subnode->token_type == RT_TERMINAL) {
                switch (subnode->terminal.terminal) {
                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN): {
                        if (subnode->next->token_type != RT_NONTERMINAL) type_checker_error();
                        if (subnode->next->nonterminal.nonterminal != NT_EXPRESSION) type_checker_error();

                        return subnode->next->type;
                    } break;

                    case token_number_identifier(): {
                        type_checker_variable_t * variable = type_checker_variable_registry_lookup(
                            vr,
                            token_buffer->tokens[subnode->terminal.position].identifier_data->name,
                            subnode->scope
                        );

                        if (variable == NULL) {
                            fatal_line_full_error(
                                line_buffer,
                                token_buffer->tokens[expression_node->nonterminal.tree.head->next->terminal.position].file_name,
                                "Undefined reference",
                                token_buffer->tokens[expression_node->nonterminal.tree.head->next->terminal.position].line_index
                            );
                        }

                        return variable->type;
                    } break;

                    case token_number_constant():
                    case token_number_string_literal(): {
                        return subnode->type;
                    } break;
                }
            }
            else type_checker_error();
        } break;

        case NT_EXPRESSION_LEVEL_1: {
            return expression_level_1_parse(vr, line_buffer, token_buffer, subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_2: {
            if (subnode->token_type == RT_NONTERMINAL) {
                return subnode->type;
            }
            else {
                switch (subnode->terminal.terminal) {
                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_INCREMENT):
                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DECREMENT):
                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_NOT):
                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PLUS):
                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MINUS): {
                        return subnode->next->type;
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LOGICAL_NEGATION): {
                        return bool_type;
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN): {
                        return subnode->next->type;
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_STAR): {
                        // TODO: remove pointer
                    } break;

                    case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_AND): {
                        // TODO: add pointer
                    } break;

                    case token_number_keyword(SCANNER_KEYWORD_TYPE_SIZEOF): {
                        return ulonglong_type;
                    }
                }
            }
        } break;

        case NT_EXPRESSION_LEVEL_3: {
            return expression_level_3_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_4: {
            return expression_level_4_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_5: {
            return expression_level_5_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_6: {
            return expression_level_6_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_7: {
            return expression_level_7_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_8: {
            return expression_level_8_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_9: {
            return expression_level_9_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_10: {
            return expression_level_10_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_11: {
            return expression_level_11_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_12: {
            return expression_level_12_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_13: {
            return expression_level_13_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_14: {
            return expression_level_14_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION_LEVEL_15: {
            return expression_level_15_parse(subnode->next, expression_node->nonterminal.tree.tail, subnode);
        } break;

        case NT_EXPRESSION: {
            return expression_top_level_parse(subnode);
        } break;

        default: break;
    }

    return NULL;
}