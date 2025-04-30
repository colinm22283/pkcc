#include <parser/rules.h>

rule_token_t empty_tokens[] = { };

rule_token_t start_decl_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_START),
};
rule_token_t start_end_tokens[] = {
    DEFINE_END_RULE_TOKEN(),
};

rule_token_t sdecl_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
};

rule_token_t vdecl_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
};

rule_token_t fdecl_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_FUNC),
};

rule_token_t decl_var_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR_NAME_LIST),
};

rule_token_t decl_var_name_list_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR_DECL),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR_LIST),
};

rule_token_t decl_var_list_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR_DECL),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR_LIST),
};

rule_token_t decl_var_decl_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR_DECL_ARRAY),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR_DECL_EQUAL),
};
rule_token_t decl_var_decl_equal_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14),
};

rule_token_t decl_var_decl_array_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SQUARE_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_OPTIONAL_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SQUARE_CLOSE)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR_DECL_ARRAY),
};

rule_token_t decl_func_proto_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_VOID),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_FUNC_ARG_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
};

rule_token_t decl_func_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_VOID),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_FUNC_ARG_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CODE_BLOCK),
};

rule_token_t decl_func_arg_list_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR_DECL_ARRAY),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_FUNC_ARG_LIST_EXTRA),
};
rule_token_t decl_func_arg_list_extra_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR_DECL_ARRAY),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_FUNC_ARG_LIST_EXTRA),
};

rule_token_t code_block_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CODE_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE)),
};

rule_token_t code_list_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CODE_LINE),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CODE_LIST),
};

rule_token_t code_line_var[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
};
rule_token_t code_line_expression[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STATEMENT),
};
rule_token_t code_line_block[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CODE_BLOCK),
};

rule_token_t statement_empty_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
};

rule_token_t statement_code_block_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CODE_BLOCK),
};

rule_token_t statement_expression_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
};

rule_token_t statement_break_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_BREAK)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
};

rule_token_t statement_continue_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_CONTINUE)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
};

rule_token_t statement_return_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_RETURN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STATEMENT_RETURN_VALUE),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
};

rule_token_t statement_if_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_IF)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STATEMENT),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STATEMENT_IF_ELSE),
};

rule_token_t statement_for_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_FOR)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STATEMENT_FOR_PARAM_1),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_OPTIONAL_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_OPTIONAL_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STATEMENT),
};

rule_token_t statement_while_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_WHILE)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STATEMENT),
};

rule_token_t statement_do_while_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_DO)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STATEMENT),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_WHILE)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
};

rule_token_t statement_return_value_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
};

rule_token_t statement_if_else_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_ELSE)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STATEMENT),
};

rule_token_t statement_for_param_1_expression_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
};
rule_token_t statement_for_param_1_decl_var_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR),
};

rule_token_t optional_expression_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
};

rule_token_t expression_drop_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_15),
};
rule_token_t expression_paren_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
};

rule_token_t expression_level_15_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_15_PRIME),
};
rule_token_t expression_level_15_comma_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_15_PRIME),
};

rule_token_t expression_level_14_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_direct_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_addition_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_ADDITION_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_subtraction_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SUBTRACTION_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_multiplication_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MULTIPLICATION_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_division_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIVISION_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_modulo_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MODULO_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_left_shift_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_right_shift_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_and_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_AND_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_xor_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_XOR_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};
rule_token_t expression_level_14_or_assignment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_OR_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14_PRIME),
};

rule_token_t expression_level_13_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_12),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13_PRIME),
};
rule_token_t expression_level_13_ternary_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_QUESTION_MARK)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COLON)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_13_PRIME),
};

rule_token_t expression_level_12_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_11),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_12_PRIME),
};
rule_token_t expression_level_12_logical_or_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LOGICAL_OR)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_11),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_12_PRIME),
};

rule_token_t expression_level_11_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_10),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_11_PRIME),
};
rule_token_t expression_level_11_logical_and_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LOGICAL_AND)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_10),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_11_PRIME),
};

rule_token_t expression_level_10_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_9),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_10_PRIME),
};
rule_token_t expression_level_10_bitwise_or_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_OR)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_9),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_10_PRIME),
};

rule_token_t expression_level_9_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_8),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_9_PRIME),
};
rule_token_t expression_level_9_bitwise_xor_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_XOR)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_8),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_9_PRIME),
};

rule_token_t expression_level_8_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_7),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_8_PRIME),
};
rule_token_t expression_level_8_bitwise_and_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_AND)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_7),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_8_PRIME),
};

rule_token_t expression_level_7_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_6),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_7_PRIME),
};
rule_token_t expression_level_7_equal_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_EQUAL_TO)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_6),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_7_PRIME),
};
rule_token_t expression_level_7_not_equal_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_NOT_EQUAL_TO)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_6),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_7_PRIME),
};

rule_token_t expression_level_6_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_5),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_6_PRIME),
};
rule_token_t expression_level_6_less_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LESS_THAN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_5),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_6_PRIME),
};
rule_token_t expression_level_6_less_equal_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LESS_THAN_OR_EQUAL_TO)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_5),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_6_PRIME),
};
rule_token_t expression_level_6_greater_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_GREATER_THAN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_5),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_6_PRIME),
};
rule_token_t expression_level_6_greater_equal_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_GREATER_THAN_OR_EQUAL_TO)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_5),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_6_PRIME),
};

rule_token_t expression_level_5_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_4),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_5_PRIME),
};
rule_token_t expression_level_5_shift_left_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_4),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_5_PRIME),
};
rule_token_t expression_level_5_shift_right_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_4),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_5_PRIME),
};

rule_token_t expression_level_4_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_3),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_4_PRIME),
};
rule_token_t expression_level_4_add_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PLUS)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_3),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_4_PRIME),
};
rule_token_t expression_level_4_sub_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MINUS)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_3),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_4_PRIME),
};

rule_token_t expression_level_3_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_3_PRIME),
};
rule_token_t expression_level_3_mul_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_STAR)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_3_PRIME),
};
rule_token_t expression_level_3_div_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SLASH)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_3_PRIME),
};
rule_token_t expression_level_3_mod_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PERCENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_3_PRIME),
};

rule_token_t expression_level_2_drop_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_1),
};
rule_token_t expression_level_2_increment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_INCREMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
};
rule_token_t expression_level_2_decrement_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DECREMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
};
rule_token_t expression_level_2_plus_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PLUS)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
};
rule_token_t expression_level_2_minus_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MINUS)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
};
rule_token_t expression_level_2_logical_not_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LOGICAL_NEGATION)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
};
rule_token_t expression_level_2_bitwise_not_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_NOT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
};
rule_token_t expression_level_2_cast_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
};
rule_token_t expression_level_2_dereference_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_STAR)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
};
rule_token_t expression_level_2_address_of_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_AND)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
};
rule_token_t expression_level_2_sizeof_type_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_SIZEOF)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
};
rule_token_t expression_level_2_sizeof_expr_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_SIZEOF)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_2),
};

rule_token_t expression_level_1_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_0),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_1_PRIME),
};
rule_token_t expression_level_1_increment_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_INCREMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_1_PRIME),
};
rule_token_t expression_level_1_decrement_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DECREMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_1_PRIME),
};
rule_token_t expression_level_1_call_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CALL_ARG_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_1_PRIME),
};
rule_token_t expression_level_1_subscript_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SQUARE_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SQUARE_CLOSE)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_1_PRIME),
};
rule_token_t expression_level_1_member_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PERIOD)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_1_PRIME),
};
rule_token_t expression_level_1_pointer_member_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DEREFERENCE)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_1_PRIME),
};
// TODO: add compound literal

rule_token_t expression_level_0_expression_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
};
rule_token_t expression_level_0_identifier_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
};
rule_token_t expression_level_0_constant_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_constant()),
};
rule_token_t expression_level_0_string_literal_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_string_literal()),
};

rule_token_t call_arg_list_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CALL_ARG_LIST_EXTRA),
};
rule_token_t call_arg_list_extra_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION_LEVEL_14),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CALL_ARG_LIST_EXTRA),
};

rule_token_t struct_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_STRUCT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_NAME),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_MEMBER_LIST),
};
rule_token_t struct_member_list_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_MEMBER),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE)),
};
rule_token_t struct_member_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECL_VAR),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_MEMBER),
};
rule_token_t struct_name_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
};

rule_token_t type_void_drop_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE),
};
rule_token_t type_void_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_VOID)),
};

rule_token_t type_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_QUALIFIER),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_INNER),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_QUALIFIER),
};

rule_token_t type_bool_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_BOOL)),
};
rule_token_t type_char_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_CHAR)),
};
rule_token_t type_short_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_SHORT)),
};
rule_token_t type_int_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_INT)),
};
rule_token_t type_long_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_LONG)),
};
rule_token_t type_float_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_FLOAT)),
};
rule_token_t type_double_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_DOUBLE)),
};
rule_token_t type_ident_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
};
rule_token_t type_struct_tokens[] = {
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT),
};

rule_token_t qualifier_const_tokens[] = {
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_CONST)),
};

rule_t all_rules[RULE_COUNT] = {
    DEFINE_RULE(NT_START, start_decl_tokens),
    DEFINE_RULE(NT_START, start_end_tokens),

    DEFINE_RULE(NT_DECL, sdecl_tokens),
    DEFINE_RULE(NT_DECL, vdecl_tokens),
    DEFINE_RULE(NT_DECL, fdecl_tokens),

    DEFINE_RULE(NT_DECL_VAR, decl_var_tokens),

    DEFINE_RULE(NT_DECL_VAR_NAME_LIST, decl_var_name_list_tokens),
    DEFINE_RULE(NT_DECL_VAR_NAME_LIST, empty_tokens),

    DEFINE_RULE(NT_DECL_VAR_LIST, decl_var_list_tokens),
    DEFINE_RULE(NT_DECL_VAR_LIST, empty_tokens),

    DEFINE_RULE(NT_DECL_VAR_DECL, decl_var_decl_tokens),

    DEFINE_RULE(NT_DECL_VAR_DECL_EQUAL, decl_var_decl_equal_tokens),
    DEFINE_RULE(NT_DECL_VAR_DECL_EQUAL, empty_tokens),

    DEFINE_RULE(NT_DECL_VAR_DECL_ARRAY, decl_var_decl_array_tokens),
    DEFINE_RULE(NT_DECL_VAR_DECL_ARRAY, empty_tokens),

    DEFINE_RULE(NT_DECL_FUNC, decl_func_proto_tokens),
    DEFINE_RULE(NT_DECL_FUNC, decl_func_tokens),

    DEFINE_RULE(NT_DECL_FUNC_ARG_LIST, decl_func_arg_list_tokens),
    DEFINE_RULE(NT_DECL_FUNC_ARG_LIST, empty_tokens),

    DEFINE_RULE(NT_DECL_FUNC_ARG_LIST_EXTRA, decl_func_arg_list_extra_tokens),
    DEFINE_RULE(NT_DECL_FUNC_ARG_LIST_EXTRA, empty_tokens),

    DEFINE_RULE(NT_CODE_BLOCK, code_block_tokens),

    DEFINE_RULE(NT_CODE_LIST, code_list_tokens),
    DEFINE_RULE(NT_CODE_LIST, empty_tokens),

    DEFINE_RULE(NT_CODE_LINE, code_line_expression),
    DEFINE_RULE(NT_CODE_LINE, code_line_var),
    DEFINE_RULE(NT_CODE_LINE, code_line_block),

    DEFINE_RULE(NT_STATEMENT, statement_empty_tokens),
    DEFINE_RULE(NT_STATEMENT, statement_code_block_tokens),
    DEFINE_RULE(NT_STATEMENT, statement_expression_tokens),
    DEFINE_RULE(NT_STATEMENT, statement_break_tokens),
    DEFINE_RULE(NT_STATEMENT, statement_continue_tokens),
    DEFINE_RULE(NT_STATEMENT, statement_return_tokens),
    DEFINE_RULE(NT_STATEMENT, statement_if_tokens),
    DEFINE_RULE(NT_STATEMENT, statement_for_tokens),
    DEFINE_RULE(NT_STATEMENT, statement_while_tokens),
    DEFINE_RULE(NT_STATEMENT, statement_do_while_tokens),

    DEFINE_RULE(NT_STATEMENT_RETURN_VALUE, statement_return_value_tokens),
    DEFINE_RULE(NT_STATEMENT_RETURN_VALUE, empty_tokens),

    DEFINE_RULE(NT_STATEMENT_IF_ELSE, statement_if_else_tokens),
    DEFINE_RULE(NT_STATEMENT_IF_ELSE, empty_tokens),

    DEFINE_RULE(NT_STATEMENT_FOR_PARAM_1, statement_for_param_1_expression_tokens),
    DEFINE_RULE(NT_STATEMENT_FOR_PARAM_1, statement_for_param_1_decl_var_tokens),
    DEFINE_RULE(NT_STATEMENT_FOR_PARAM_1, empty_tokens),

    DEFINE_RULE(NT_OPTIONAL_EXPRESSION, optional_expression_tokens),
    DEFINE_RULE(NT_OPTIONAL_EXPRESSION, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION, expression_drop_tokens),
    DEFINE_RULE(NT_EXPRESSION, expression_paren_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_15, expression_level_15_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_15_PRIME, expression_level_15_comma_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_15_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_14, expression_level_14_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_direct_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_addition_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_subtraction_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_multiplication_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_division_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_modulo_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_left_shift_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_right_shift_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_and_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_xor_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, expression_level_14_or_assignment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_14_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_13, expression_level_13_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_13_PRIME, expression_level_13_ternary_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_13_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_13, expression_level_13_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_13_PRIME, expression_level_13_ternary_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_13_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_12, expression_level_12_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_12_PRIME, expression_level_12_logical_or_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_12_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_11, expression_level_11_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_11_PRIME, expression_level_11_logical_and_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_11_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_10, expression_level_10_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_10_PRIME, expression_level_10_bitwise_or_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_10_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_9, expression_level_9_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_9_PRIME, expression_level_9_bitwise_xor_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_9_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_8, expression_level_8_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_8_PRIME, expression_level_8_bitwise_and_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_8_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_7, expression_level_7_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_7_PRIME, expression_level_7_equal_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_7_PRIME, expression_level_7_not_equal_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_7_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_6, expression_level_6_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_6_PRIME, expression_level_6_less_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_6_PRIME, expression_level_6_less_equal_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_6_PRIME, expression_level_6_greater_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_6_PRIME, expression_level_6_greater_equal_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_6_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_5, expression_level_5_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_5_PRIME, expression_level_5_shift_left_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_5_PRIME, expression_level_5_shift_right_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_5_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_4, expression_level_4_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_4_PRIME, expression_level_4_add_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_4_PRIME, expression_level_4_sub_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_4_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_3, expression_level_3_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_3_PRIME, expression_level_3_mul_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_3_PRIME, expression_level_3_div_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_3_PRIME, expression_level_3_mod_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_3_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_drop_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_increment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_decrement_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_plus_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_minus_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_logical_not_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_bitwise_not_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_cast_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_dereference_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_address_of_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_sizeof_type_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_2, expression_level_2_sizeof_expr_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_1, expression_level_1_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_1_PRIME, expression_level_1_increment_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_1_PRIME, expression_level_1_decrement_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_1_PRIME, expression_level_1_call_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_1_PRIME, expression_level_1_subscript_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_1_PRIME, expression_level_1_member_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_1_PRIME, expression_level_1_pointer_member_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_1_PRIME, empty_tokens),

    DEFINE_RULE(NT_EXPRESSION_LEVEL_0, expression_level_0_expression_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_0, expression_level_0_identifier_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_0, expression_level_0_constant_tokens),
    DEFINE_RULE(NT_EXPRESSION_LEVEL_0, expression_level_0_string_literal_tokens),

    DEFINE_RULE(NT_CALL_ARG_LIST, call_arg_list_tokens),
    DEFINE_RULE(NT_CALL_ARG_LIST, empty_tokens),
    DEFINE_RULE(NT_CALL_ARG_LIST_EXTRA, call_arg_list_extra_tokens),
    DEFINE_RULE(NT_CALL_ARG_LIST_EXTRA, empty_tokens),

    DEFINE_RULE(NT_STRUCT, struct_tokens),
    DEFINE_RULE(NT_STRUCT_MEMBER_LIST, struct_member_list_tokens),
    DEFINE_RULE(NT_STRUCT_MEMBER_LIST, empty_tokens),
    DEFINE_RULE(NT_STRUCT_MEMBER, struct_member_tokens),
    DEFINE_RULE(NT_STRUCT_MEMBER, empty_tokens),
    DEFINE_RULE(NT_STRUCT_NAME, struct_name_tokens),
    DEFINE_RULE(NT_STRUCT_NAME, empty_tokens),

    DEFINE_RULE(NT_TYPE_VOID, type_void_drop_tokens),
    DEFINE_RULE(NT_TYPE_VOID, type_void_tokens),

    DEFINE_RULE(NT_TYPE, type_tokens),

    DEFINE_RULE(NT_TYPE_INNER, type_bool_tokens),
    DEFINE_RULE(NT_TYPE_INNER, type_char_tokens),
    DEFINE_RULE(NT_TYPE_INNER, type_short_tokens),
    DEFINE_RULE(NT_TYPE_INNER, type_int_tokens),
    DEFINE_RULE(NT_TYPE_INNER, type_long_tokens),
    DEFINE_RULE(NT_TYPE_INNER, type_float_tokens),
    DEFINE_RULE(NT_TYPE_INNER, type_double_tokens),
    DEFINE_RULE(NT_TYPE_INNER, type_ident_tokens),
    DEFINE_RULE(NT_TYPE_INNER, type_struct_tokens),

    DEFINE_RULE(NT_QUALIFIER, qualifier_const_tokens),
    DEFINE_RULE(NT_QUALIFIER, empty_tokens),
};
