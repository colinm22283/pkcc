#include <scanner/token_strings.h>

keyword_string_entry_t keyword_strings[KEYWORD_STRINGS_COUNT] = {
    (keyword_string_entry_t) {
        .string = "imaginary",
        .length = 9,
        .keyword = SCANNER_KEYWORD_TYPE_IMAGINARY,
    },
    (keyword_string_entry_t) {
        .string = "continue",
        .length = 8,
        .keyword = SCANNER_KEYWORD_TYPE_CONTINUE,
    },
    (keyword_string_entry_t) {
        .string = "register",
        .length = 8,
        .keyword = SCANNER_KEYWORD_TYPE_REGISTER,
    },
    (keyword_string_entry_t) {
        .string = "restrict",
        .length = 8,
        .keyword = SCANNER_KEYWORD_TYPE_RESTRICT,
    },
    (keyword_string_entry_t) {
        .string = "unsigned",
        .length = 8,
        .keyword = SCANNER_KEYWORD_TYPE_UNSIGNED,
    },
    (keyword_string_entry_t) {
        .string = "volatile",
        .length = 8,
        .keyword = SCANNER_KEYWORD_TYPE_VOLATILE,
    },
    (keyword_string_entry_t) {
        .string = "default",
        .length = 7,
        .keyword = SCANNER_KEYWORD_TYPE_DEFAULT,
    },
    (keyword_string_entry_t) {
        .string = "typedef",
        .length = 7,
        .keyword = SCANNER_KEYWORD_TYPE_TYPEDEF,
    },
    (keyword_string_entry_t) {
        .string = "complex",
        .length = 7,
        .keyword = SCANNER_KEYWORD_TYPE_COMPLEX,
    },
    (keyword_string_entry_t) {
        .string = "double",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_DOUBLE,
    },
    (keyword_string_entry_t) {
        .string = "extern",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_EXTERN,
    },
    (keyword_string_entry_t) {
        .string = "inline",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_INLINE,
    },
    (keyword_string_entry_t) {
        .string = "return",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_RETURN,
    },
    (keyword_string_entry_t) {
        .string = "signed",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_SIGNED,
    },
    (keyword_string_entry_t) {
        .string = "sizeof",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_SIZEOF,
    },
    (keyword_string_entry_t) {
        .string = "static",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_STATIC,
    },
    (keyword_string_entry_t) {
        .string = "struct",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_STRUCT,
    },
    (keyword_string_entry_t) {
        .string = "switch",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_SWITCH,
    },
    (keyword_string_entry_t) {
        .string = "break",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_BREAK,
    },
    (keyword_string_entry_t) {
        .string = "const",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_CONST,
    },
    (keyword_string_entry_t) {
        .string = "float",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_FLOAT,
    },
    (keyword_string_entry_t) {
        .string = "short",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_SHORT,
    },
    (keyword_string_entry_t) {
        .string = "union",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_UNION,
    },
    (keyword_string_entry_t) {
        .string = "while",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_WHILE,
    },
    (keyword_string_entry_t) {
        .string = "case",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_CASE,
    },
    (keyword_string_entry_t) {
        .string = "char",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_CHAR,
    },
    (keyword_string_entry_t) {
        .string = "else",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_ELSE,
    },
    (keyword_string_entry_t) {
        .string = "enum",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_ENUM,
    },
    (keyword_string_entry_t) {
        .string = "goto",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_GOTO,
    },
    (keyword_string_entry_t) {
        .string = "long",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_LONG,
    },
    (keyword_string_entry_t) {
        .string = "void",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_VOID,
    },
    (keyword_string_entry_t) {
        .string = "bool",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_BOOL,
    },
    (keyword_string_entry_t) {
        .string = "for",
        .length = 3,
        .keyword = SCANNER_KEYWORD_TYPE_FOR,
    },
    (keyword_string_entry_t) {
        .string = "int",
        .length = 3,
        .keyword = SCANNER_KEYWORD_TYPE_INT,
    },
    (keyword_string_entry_t) {
        .string = "do",
        .length = 2,
        .keyword = SCANNER_KEYWORD_TYPE_DO,
    },
    (keyword_string_entry_t) {
        .string = "if",
        .length = 2,
        .keyword = SCANNER_KEYWORD_TYPE_IF,
    },
};

punctuation_string_entry_t punctuation_strings[PUNCTUATION_STRINGS_COUNT] = {
    (punctuation_string_entry_t) {
        .string = "<<=",
        .length = 3,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = ">>=",
        .length = 3,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = "->*",
        .length = 3,
        .punctuation = SCANNER_PUNCTUATION_TYPE_POINTER_TO_DEREFERENCE,
    },
    (punctuation_string_entry_t) {
        .string = "...",
        .length = 3,
        .punctuation = SCANNER_PUNCTUATION_TYPE_ELLIPSIS,
    },
    (punctuation_string_entry_t) {
        .string = "++",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_INCREMENT,
    },
    (punctuation_string_entry_t) {
        .string = "--",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_DECREMENT,
    },
    (punctuation_string_entry_t) {
        .string = "==",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_EQUAL_TO,
    },
    (punctuation_string_entry_t) {
        .string = "!=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_NOT_EQUAL_TO,
    },
    (punctuation_string_entry_t) {
        .string = ">=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_GREATER_THAN_OR_EQUAL_TO,
    },
    (punctuation_string_entry_t) {
        .string = "<=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_LESS_THAN_OR_EQUAL_TO,
    },
    (punctuation_string_entry_t) {
        .string = "&&",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_LOGICAL_AND,
    },
    (punctuation_string_entry_t) {
        .string = "||",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_LOGICAL_OR,
    },
    (punctuation_string_entry_t) {
        .string = "|",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_OR,
    },
    (punctuation_string_entry_t) {
        .string = "<<",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT,
    },
    (punctuation_string_entry_t) {
        .string = ">>",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT,
    },
    (punctuation_string_entry_t) {
        .string = "+=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_ADDITION_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = "-=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_SUBTRACTION_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = "*=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_MULTIPLICATION_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = "/=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_DIVISION_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = "%=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_MODULO_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = "&=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_AND_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = "|=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_OR_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = "^=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_XOR_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = "->",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_DEREFERENCE,
    },
    (punctuation_string_entry_t) {
        .string = ".*",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_POINTER_TO_REFERENCE,
    },
    (punctuation_string_entry_t) {
        .string = "+",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_PLUS,
    },
    (punctuation_string_entry_t) {
        .string = "-",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_MINUS,
    },
    (punctuation_string_entry_t) {
        .string = "*",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_STAR,
    },
    (punctuation_string_entry_t) {
        .string = "/",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_SLASH,
    },
    (punctuation_string_entry_t) {
        .string = "%",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_PERCENT,
    },
    (punctuation_string_entry_t) {
        .string = ">",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_GREATER_THAN,
    },
    (punctuation_string_entry_t) {
        .string = "<",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_LESS_THAN,
    },
    (punctuation_string_entry_t) {
        .string = "!",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_LOGICAL_NEGATION,
    },
    (punctuation_string_entry_t) {
        .string = "~",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_NOT,
    },
    (punctuation_string_entry_t) {
        .string = "&",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_AND,
    },
    (punctuation_string_entry_t) {
        .string = "^",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_XOR,
    },
    (punctuation_string_entry_t) {
        .string = "=",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT,
    },
    (punctuation_string_entry_t) {
        .string = "{",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_CURLY_OPEN,
    },
    (punctuation_string_entry_t) {
        .string = "}",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE,
    },
    (punctuation_string_entry_t) {
        .string = "[",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_SQUARE_OPEN,
    },
    (punctuation_string_entry_t) {
        .string = "]",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_SQUARE_CLOSE,
    },
    (punctuation_string_entry_t) {
        .string = "(",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_PAREN_OPEN,
    },
    (punctuation_string_entry_t) {
        .string = ")",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE,
    },
    (punctuation_string_entry_t) {
        .string = ";",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_SEMICOLON,
    },
    (punctuation_string_entry_t) {
        .string = "?",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_QUESTION_MARK,
    },
    (punctuation_string_entry_t) {
        .string = ".",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_PERIOD,
    },
    (punctuation_string_entry_t) {
        .string = ",",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_COMMA,
    },
};