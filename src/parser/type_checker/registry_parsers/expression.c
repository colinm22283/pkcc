#include <parser/type_checker/registry_parsers/expression.h>
#include <parser/type_checker/error.h>

#include <debug/log.h>

type_checker_type_t * expression_level_1_parse(
    syntax_tree_node_list_node_t * node,
    syntax_tree_node_list_node_t * tail_node,
    type_checker_type_t * base_type
) {
    {
        syntax_tree_node_list_node_t * temp = node;
        while (temp != tail_node) {
            if (temp->token_type == RT_NONTERMINAL) {
                if (temp->nonterminal.nonterminal == NT_EXPRESSION_LEVEL_1_PRIME) {
                    base_type = expression_level_1_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, base_type);
                    temp->type = base_type;

                    break;
                }
            }

            temp = temp->next;
        }
    }

    switch (node->terminal.terminal) {
        // TODO: function calls and stuff
        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_INCREMENT):
        case token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DECREMENT): {
            return base_type;
        } break;

        default: break;
    }

    return NULL;
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

                    return expression_level_3_parse(temp->nonterminal.tree.head->next, temp->nonterminal.tree.tail, temp);
                }
            }

            temp = temp->next;
        }
    }

    return base_node->type;
}

type_checker_type_t * type_checker_registry_parse_expression(
    type_checker_registry_t * tr,
    line_buffer_t * line_buffer,
    token_buffer_t * token_buffer,
    syntax_tree_node_list_node_t * expression_node
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

                    case token_number_identifier():
                    case token_number_constant():
                    case token_number_string_literal(): {
                        return subnode->type;
                    } break;
                }
            }
            else type_checker_error();
        } break;

        case NT_EXPRESSION_LEVEL_1: {
            type_checker_type_t * subtype = subnode->type;

            if (subnode->next != expression_node->nonterminal.tree.tail) {
                subnode->next->type = expression_level_1_parse(subnode->next->nonterminal.tree.head->next, subnode->next->nonterminal.tree.tail, subtype);
                return subnode->next->type;
            }
            else return subtype;
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

        default: break;
    }

    return NULL;
}