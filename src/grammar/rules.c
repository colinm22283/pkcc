#include <scanner/token.h>

#include <grammar/rules.h>
#include <grammar/nonterminals.h>

DEFINE_RULE_TOKENS(tokens_empty);

DEFINE_RULE_TOKENS(tokens_tranlation_unit_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXTERNAL_DECLARATION)
);
DEFINE_RULE_TOKENS(tokens_tranlation_unit_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TRANSLATION_UNIT),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXTERNAL_DECLARATION)
);

DEFINE_RULE_TOKENS(tokens_primary_expression_0,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier())
);
DEFINE_RULE_TOKENS(tokens_primary_expression_1,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_constant())
);
DEFINE_RULE_TOKENS(tokens_primary_expression_2,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_string_literal())
);
DEFINE_RULE_TOKENS(tokens_primary_expression_3,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE))
);

DEFINE_RULE_TOKENS(tokens_postfix_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_PRIMARY_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_postfix_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_POSTFIX_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SQUARE_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SQUARE_CLOSE))
);
DEFINE_RULE_TOKENS(tokens_postfix_expression_2,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_POSTFIX_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE))
);
DEFINE_RULE_TOKENS(tokens_postfix_expression_3,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_POSTFIX_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ARGUMENT_EXPRESSION_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE))
);
DEFINE_RULE_TOKENS(tokens_postfix_expression_4,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_POSTFIX_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PERIOD)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier())
);
DEFINE_RULE_TOKENS(tokens_postfix_expression_5,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_POSTFIX_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DEREFERENCE)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier())
);
DEFINE_RULE_TOKENS(tokens_postfix_expression_6,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_POSTFIX_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_INCREMENT))
);
DEFINE_RULE_TOKENS(tokens_postfix_expression_7,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_POSTFIX_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DECREMENT))
);
DEFINE_RULE_TOKENS(tokens_postfix_expression_8,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_NAME),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_INITIALIZER_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE))
);
DEFINE_RULE_TOKENS(tokens_postfix_expression_9,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_NAME),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_INITIALIZER_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE))
);

DEFINE_RULE_TOKENS(tokens_argument_expression_list_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ASSIGNMENT_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_argument_expression_list_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ARGUMENT_EXPRESSION_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ASSIGNMENT_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_unary_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_POSTFIX_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_unary_expression_1,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_INCREMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_UNARY_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_unary_expression_2,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DECREMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_UNARY_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_unary_expression_3,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_UNARY_OPERATOR),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CAST_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_unary_expression_4,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_SIZEOF)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_UNARY_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_unary_expression_5,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_SIZEOF)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_UNARY_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE))
);

DEFINE_RULE_TOKENS(tokens_unary_operator_0, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_AND)));
DEFINE_RULE_TOKENS(tokens_unary_operator_1, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_STAR)));
DEFINE_RULE_TOKENS(tokens_unary_operator_2, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PLUS)));
DEFINE_RULE_TOKENS(tokens_unary_operator_3, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MINUS)));
DEFINE_RULE_TOKENS(tokens_unary_operator_4, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_NOT)));
DEFINE_RULE_TOKENS(tokens_unary_operator_5, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LOGICAL_NEGATION)));

DEFINE_RULE_TOKENS(tokens_cast_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_UNARY_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_cast_expression_1,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_NAME),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CAST_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_multiplicative_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CAST_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_multiplicative_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_MULTIPLICATIVE_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_STAR)),
);
DEFINE_RULE_TOKENS(tokens_multiplicative_expression_2,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_MULTIPLICATIVE_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SLASH)),
);
DEFINE_RULE_TOKENS(tokens_multiplicative_expression_3,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_MULTIPLICATIVE_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PERCENT)),
);

DEFINE_RULE_TOKENS(tokens_additive_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_MULTIPLICATIVE_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_additive_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ADDITIVE_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PLUS)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_MULTIPLICATIVE_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_additive_expression_2,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ADDITIVE_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MINUS)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_MULTIPLICATIVE_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_shift_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ADDITIVE_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_shift_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SHIFT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ADDITIVE_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_shift_expression_2,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SHIFT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ADDITIVE_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_relational_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SHIFT_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_relational_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_RELATIONAL_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LESS_THAN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SHIFT_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_relational_expression_2,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_RELATIONAL_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_GREATER_THAN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SHIFT_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_relational_expression_3,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_RELATIONAL_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LESS_THAN_OR_EQUAL_TO)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SHIFT_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_relational_expression_4,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_RELATIONAL_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_GREATER_THAN_OR_EQUAL_TO)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SHIFT_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_equality_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_RELATIONAL_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_equality_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EQUALITY_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_EQUAL_TO)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_RELATIONAL_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_equality_expression_2,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EQUALITY_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_NOT_EQUAL_TO)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_RELATIONAL_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_and_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EQUALITY_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_and_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_AND_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_AND)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EQUALITY_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_xor_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_AND_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_xor_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_XOR_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_XOR)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_AND_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_or_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_XOR_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_or_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_OR_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_OR)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_XOR_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_logical_and_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_OR_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_logical_and_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_LOGICAL_AND_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LOGICAL_AND)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_OR_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_logical_or_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_LOGICAL_AND_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_logical_or_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_LOGICAL_OR_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_LOGICAL_OR)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_LOGICAL_AND_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_conditional_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_LOGICAL_OR_EXPRESSION)
    );
DEFINE_RULE_TOKENS(tokens_conditional_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_LOGICAL_OR_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_QUESTION_MARK)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COLON)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CONDITIONAL_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_assignment_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CONDITIONAL_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_assignment_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_UNARY_EXPRESSION),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ASSIGNMENT_OPERATOR),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ASSIGNMENT_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_assignment_operator_0, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT)));
DEFINE_RULE_TOKENS(tokens_assignment_operator_1, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MULTIPLICATION_ASSIGNMENT)));
DEFINE_RULE_TOKENS(tokens_assignment_operator_2, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIVISION_ASSIGNMENT)));
DEFINE_RULE_TOKENS(tokens_assignment_operator_3, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_MODULO_ASSIGNMENT)));
DEFINE_RULE_TOKENS(tokens_assignment_operator_4, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_ADDITION_ASSIGNMENT)));
DEFINE_RULE_TOKENS(tokens_assignment_operator_5, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SUBTRACTION_ASSIGNMENT)));
DEFINE_RULE_TOKENS(tokens_assignment_operator_6, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT_ASSIGNMENT)));
DEFINE_RULE_TOKENS(tokens_assignment_operator_7, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT_ASSIGNMENT)));
DEFINE_RULE_TOKENS(tokens_assignment_operator_8, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_AND_ASSIGNMENT)));
DEFINE_RULE_TOKENS(tokens_assignment_operator_9, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_XOR_ASSIGNMENT)));
DEFINE_RULE_TOKENS(tokens_assignment_operator_10, DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_BITWISE_OR_ASSIGNMENT)));

DEFINE_RULE_TOKENS(tokens_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ASSIGNMENT_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_expression_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_EXPRESSION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ASSIGNMENT_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_constant_expression_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CONDITIONAL_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_declaration_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECLARATION_SPECIFIERS),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON))
);
DEFINE_RULE_TOKENS(tokens_declaration_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECLARATION_SPECIFIERS),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_INIT_DECLARATOR_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON))
);

DEFINE_RULE_TOKENS(tokens_declaration_specifiers_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STORAGE_CLASS_SPECIFIER)
);
DEFINE_RULE_TOKENS(tokens_declaration_specifiers_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STORAGE_CLASS_SPECIFIER),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECLARATION_SPECIFIERS)
);
DEFINE_RULE_TOKENS(tokens_declaration_specifiers_2,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_SPECIFIER)
);
DEFINE_RULE_TOKENS(tokens_declaration_specifiers_3,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_SPECIFIER),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECLARATION_SPECIFIERS)
);
DEFINE_RULE_TOKENS(tokens_declaration_specifiers_4,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_QUALIFIER)
);
DEFINE_RULE_TOKENS(tokens_declaration_specifiers_5,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_QUALIFIER),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECLARATION_SPECIFIERS)
);
DEFINE_RULE_TOKENS(tokens_declaration_specifiers_6,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_FUNCTION_SPECIFIER)
);
DEFINE_RULE_TOKENS(tokens_declaration_specifiers_7,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_FUNCTION_SPECIFIER),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECLARATION_SPECIFIERS)
);

DEFINE_RULE_TOKENS(tokens_init_declarator_list_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_INIT_DECLARATOR)
);
DEFINE_RULE_TOKENS(tokens_init_declarator_list_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_INIT_DECLARATOR_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_INIT_DECLARATOR)
);

DEFINE_RULE_TOKENS(tokens_init_declarator_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECLARATOR)
);
DEFINE_RULE_TOKENS(tokens_init_declarator_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECLARATOR),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_INITIALIZER)
);

DEFINE_RULE_TOKENS(tokens_storage_class_specifier_0, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_TYPEDEF)));
DEFINE_RULE_TOKENS(tokens_storage_class_specifier_1, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_EXTERN)));
DEFINE_RULE_TOKENS(tokens_storage_class_specifier_2, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_STATIC)));
DEFINE_RULE_TOKENS(tokens_storage_class_specifier_3, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_AUTO)));
DEFINE_RULE_TOKENS(tokens_storage_class_specifier_4, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_REGISTER)));

DEFINE_RULE_TOKENS(tokens_type_specifier_0, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_VOID)));
DEFINE_RULE_TOKENS(tokens_type_specifier_1, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_CHAR)));
DEFINE_RULE_TOKENS(tokens_type_specifier_2, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_SHORT)));
DEFINE_RULE_TOKENS(tokens_type_specifier_3, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_INT)));
DEFINE_RULE_TOKENS(tokens_type_specifier_4, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_LONG)));
DEFINE_RULE_TOKENS(tokens_type_specifier_5, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_FLOAT)));
DEFINE_RULE_TOKENS(tokens_type_specifier_6, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_DOUBLE)));
DEFINE_RULE_TOKENS(tokens_type_specifier_7, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_SIGNED)));
DEFINE_RULE_TOKENS(tokens_type_specifier_8, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_UNSIGNED)));
DEFINE_RULE_TOKENS(tokens_type_specifier_9, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_BOOL)));
DEFINE_RULE_TOKENS(tokens_type_specifier_10, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_COMPLEX)));
DEFINE_RULE_TOKENS(tokens_type_specifier_11, DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_IMAGINARY)));
DEFINE_RULE_TOKENS(tokens_type_specifier_12, DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_OR_UNION_SPECIFIER));
DEFINE_RULE_TOKENS(tokens_type_specifier_13, DEFINE_NONTERMINAL_RULE_TOKEN(NT_ENUM_SPECIFIER));
DEFINE_RULE_TOKENS(tokens_type_specifier_14, DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()));

DEFINE_RULE_TOKENS(tokens_struct_or_union_specifier_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_OR_UNION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_DECLARATION_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE))
);
DEFINE_RULE_TOKENS(tokens_struct_or_union_specifier_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_OR_UNION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_DECLARATION_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE))
);
DEFINE_RULE_TOKENS(tokens_struct_or_union_specifier_2,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_OR_UNION),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier())
);

DEFINE_RULE_TOKENS(tokens_struct_or_union_0,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_STRUCT))
);
DEFINE_RULE_TOKENS(tokens_struct_or_union_1,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_UNION))
);

DEFINE_RULE_TOKENS(tokens_struct_declaration_list_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_DECLARATION)
);
DEFINE_RULE_TOKENS(tokens_struct_declaration_list_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_DECLARATION_LIST),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_DECLARATION)
);

DEFINE_RULE_TOKENS(tokens_struct_declaration_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SPECIFIER_QUALIFIER_LIST),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_DECLARATOR_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON))
);

DEFINE_RULE_TOKENS(tokens_specifier_qualifer_list_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_SPECIFIER),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SPECIFIER_QUALIFIER_LIST)
);
DEFINE_RULE_TOKENS(tokens_specifier_qualifer_list_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_SPECIFIER)
);
DEFINE_RULE_TOKENS(tokens_specifier_qualifer_list_2,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_QUALIFIER),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SPECIFIER_QUALIFIER_LIST)
);
DEFINE_RULE_TOKENS(tokens_specifier_qualifer_list_3,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_TYPE_QUALIFIER)
);

DEFINE_RULE_TOKENS(tokens_struct_declarator_list_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_DECLARATOR)
);
DEFINE_RULE_TOKENS(tokens_struct_declarator_list_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_DECLARATOR_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_STRUCT_DECLARATOR)
);

DEFINE_RULE_TOKENS(tokens_struct_declarator_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECLARATOR)
);
DEFINE_RULE_TOKENS(tokens_struct_declarator_1,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COLON)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CONSTANT_EXPRESSION)
);
DEFINE_RULE_TOKENS(tokens_struct_declarator_2,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_DECLARATOR),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COLON)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CONSTANT_EXPRESSION)
);

DEFINE_RULE_TOKENS(tokens_enum_specifier_0,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_ENUM)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ENUMERATOR_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE))
);
DEFINE_RULE_TOKENS(tokens_enum_specifier_1,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_ENUM)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ENUMERATOR_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE))
);
DEFINE_RULE_TOKENS(tokens_enum_specifier_2,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_ENUM)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ENUMERATOR_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE))
);
DEFINE_RULE_TOKENS(tokens_enum_specifier_3,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_ENUM)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_OPEN)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ENUMERATOR_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE))
);
DEFINE_RULE_TOKENS(tokens_enum_specifier_4,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_keyword(SCANNER_KEYWORD_TYPE_ENUM)),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier())
);

DEFINE_RULE_TOKENS(tokens_enumerator_list_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ENUMERATOR)
);
DEFINE_RULE_TOKENS(tokens_enumerator_list_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ENUMERATOR_LIST),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_COMMA)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_ENUMERATOR)
);

DEFINE_RULE_TOKENS(tokens_enumerator_0,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier())
    );
DEFINE_RULE_TOKENS(tokens_enumerator_0,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_CONSTANT_EXPRESSION)
);

rule_t all_rules[RULE_COUNT] = {
    DEFINE_RULE_MULTI(NT_TRANSLATION_UNIT,
        tokens_tranlation_unit_0,
        tokens_tranlation_unit_1
    ),

    DEFINE_RULE_MULTI(NT_PRIMARY_EXPRESSION,
        tokens_primary_expression_0,
        tokens_primary_expression_1,
        tokens_primary_expression_2,
        tokens_primary_expression_3
    ),

    DEFINE_RULE_MULTI(NT_POSTFIX_EXPRESSION,
        tokens_postfix_expression_0,
        tokens_postfix_expression_1,
        tokens_postfix_expression_2,
        tokens_postfix_expression_3,
        tokens_postfix_expression_4,
        tokens_postfix_expression_5,
        tokens_postfix_expression_6,
        tokens_postfix_expression_7,
        tokens_postfix_expression_8,
        tokens_postfix_expression_9
    ),

    DEFINE_RULE_MULTI(NT_ARGUMENT_EXPRESSION_LIST,
        tokens_argument_expression_list_0,
        tokens_argument_expression_list_1
    ),

    DEFINE_RULE_MULTI(NT_UNARY_EXPRESSION,
        tokens_unary_expression_0,
        tokens_unary_expression_1,
        tokens_unary_expression_2,
        tokens_unary_expression_3,
        tokens_unary_expression_4,
        tokens_unary_expression_5
    ),

    DEFINE_RULE_MULTI(NT_UNARY_OPERATOR,
        tokens_unary_operator_0,
        tokens_unary_operator_1,
        tokens_unary_operator_2,
        tokens_unary_operator_3,
        tokens_unary_operator_4,
        tokens_unary_operator_5
    ),

    DEFINE_RULE_MULTI(NT_CAST_EXPRESSION,
        tokens_cast_expression_0,
        tokens_cast_expression_1
    ),

    DEFINE_RULE_MULTI(NT_MULTIPLICATIVE_EXPRESSION,
        tokens_multiplicative_expression_0,
        tokens_multiplicative_expression_1,
        tokens_multiplicative_expression_2,
        tokens_multiplicative_expression_3
    ),

    DEFINE_RULE_MULTI(NT_ADDITIVE_EXPRESSION,
        tokens_additive_expression_0,
        tokens_additive_expression_1,
        tokens_additive_expression_2
    ),

    DEFINE_RULE_MULTI(NT_SHIFT_EXPRESSION,
        tokens_shift_expression_0,
        tokens_shift_expression_1,
        tokens_shift_expression_2
    ),

    DEFINE_RULE_MULTI(NT_RELATIONAL_EXPRESSION,
        tokens_relational_expression_0,
        tokens_relational_expression_1,
        tokens_relational_expression_2,
        tokens_relational_expression_3,
        tokens_relational_expression_4
    ),

    DEFINE_RULE_MULTI(NT_EQUALITY_EXPRESSION,
        tokens_equality_expression_0,
        tokens_equality_expression_1,
        tokens_equality_expression_2
    ),

    DEFINE_RULE_MULTI(NT_AND_EXPRESSION,
        tokens_and_expression_0,
        tokens_and_expression_1
    ),

    DEFINE_RULE_MULTI(NT_XOR_EXPRESSION,
        tokens_xor_expression_0,
        tokens_xor_expression_1
    ),

    DEFINE_RULE_MULTI(NT_OR_EXPRESSION,
        tokens_or_expression_0,
        tokens_or_expression_1
    ),

    DEFINE_RULE_MULTI(NT_LOGICAL_AND_EXPRESSION,
        tokens_logical_and_expression_0,
        tokens_logical_and_expression_1
    ),

    DEFINE_RULE_MULTI(NT_LOGICAL_OR_EXPRESSION,
        tokens_logical_or_expression_0,
        tokens_logical_or_expression_1
    ),

    DEFINE_RULE_MULTI(NT_CONDITIONAL_EXPRESSION,
        tokens_conditional_expression_0,
        tokens_conditional_expression_1
    ),

    DEFINE_RULE_MULTI(NT_ASSIGNMENT_EXPRESSION,
        tokens_assignment_expression_0,
        tokens_assignment_expression_1
    ),

    DEFINE_RULE_MULTI(NT_ASSIGNMENT_OPERATOR,
        tokens_assignment_operator_0,
        tokens_assignment_operator_1,
        tokens_assignment_operator_2,
        tokens_assignment_operator_3,
        tokens_assignment_operator_4,
        tokens_assignment_operator_5,
        tokens_assignment_operator_6,
        tokens_assignment_operator_7,
        tokens_assignment_operator_8,
        tokens_assignment_operator_9,
        tokens_assignment_operator_10
    ),

    DEFINE_RULE_MULTI(NT_EXPRESSION,
        tokens_expression_0,
        tokens_expression_1
    ),

    DEFINE_RULE_MULTI(NT_CONSTANT_EXPRESSION,
        tokens_constant_expression_0
    ),

    DEFINE_RULE_MULTI(NT_DECLARATION,
        tokens_declaration_0,
        tokens_declaration_1
    ),

    DEFINE_RULE_MULTI(NT_DECLARATION_SPECIFIERS,
        tokens_declaration_specifiers_0,
        tokens_declaration_specifiers_1,
        tokens_declaration_specifiers_2,
        tokens_declaration_specifiers_3,
        tokens_declaration_specifiers_4,
        tokens_declaration_specifiers_5,
        tokens_declaration_specifiers_6,
        tokens_declaration_specifiers_7
    ),

    DEFINE_RULE_MULTI(NT_INIT_DECLARATOR_LIST,
        tokens_init_declarator_list_0,
        tokens_init_declarator_list_1
    ),

    DEFINE_RULE_MULTI(NT_INIT_DECLARATOR,
        tokens_init_declarator_0,
        tokens_init_declarator_1
    ),

    DEFINE_RULE_MULTI(NT_STORAGE_CLASS_SPECIFIER,
        tokens_storage_class_specifier_0,
        tokens_storage_class_specifier_1,
        tokens_storage_class_specifier_2,
        tokens_storage_class_specifier_3,
        tokens_storage_class_specifier_4
    ),

    DEFINE_RULE_MULTI(NT_TYPE_SPECIFIER,
        tokens_type_specifier_0,
        tokens_type_specifier_1,
        tokens_type_specifier_2,
        tokens_type_specifier_3,
        tokens_type_specifier_4,
        tokens_type_specifier_5,
        tokens_type_specifier_6,
        tokens_type_specifier_7,
        tokens_type_specifier_8,
        tokens_type_specifier_9,
        tokens_type_specifier_10,
        tokens_type_specifier_11,
        tokens_type_specifier_12,
        tokens_type_specifier_13,
        tokens_type_specifier_14
    ),

    DEFINE_RULE_MULTI(NT_STRUCT_OR_UNION_SPECIFIER,
        tokens_struct_or_union_specifier_0,
        tokens_struct_or_union_specifier_1,
        tokens_struct_or_union_specifier_2
    ),

    DEFINE_RULE_MULTI(NT_STRUCT_OR_UNION,
        tokens_struct_or_union_0,
        tokens_struct_or_union_1
    ),

    DEFINE_RULE_MULTI(NT_STRUCT_DECLARATION_LIST,
        tokens_struct_declaration_list_0,
        tokens_struct_declaration_list_1
    ),

    DEFINE_RULE_MULTI(NT_STRUCT_DECLARATION,
        tokens_struct_declaration_0
    ),

    DEFINE_RULE_MULTI(NT_SPECIFIER_QUALIFIER_LIST,
        tokens_specifier_qualifer_list_0,
        tokens_specifier_qualifer_list_1,
        tokens_specifier_qualifer_list_2,
        tokens_specifier_qualifer_list_3
    ),

    DEFINE_RULE_MULTI(NT_STRUCT_DECLARATOR_LIST,
        tokens_struct_declarator_list_0,
        tokens_struct_declarator_list_1
    ),

    DEFINE_RULE_MULTI(NT_STRUCT_DECLARATOR,
        tokens_struct_declarator_0,
        tokens_struct_declarator_1,
        tokens_struct_declarator_2
    ),

    DEFINE_RULE_MULTI(NT_ENUM_SPECIFIER,
        tokens_enum_specifier_0,
        tokens_enum_specifier_1,
        tokens_enum_specifier_2,
        tokens_enum_specifier_3,
        tokens_enum_specifier_4
    ),

    DEFINE_RULE_MULTI(NT_ENUMERATOR_LIST,
        tokens_enumerator_list_0,
        tokens_enumerator_list_1
    ),
};
