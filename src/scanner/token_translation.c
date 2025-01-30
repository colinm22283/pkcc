#include <scanner/token_translation.h>

keyword_translation_entry_t keyword_translations[KEYWORD_TRANSLATIONS_COUNT] = {
    (keyword_translation_entry_t) {
        .string = "imaginary",
        .length = 9,
        .keyword = SCANNER_KEYWORD_TYPE_IMAGINARY,
    },
    (keyword_translation_entry_t) {
        .string = "continue",
        .length = 8,
        .keyword = SCANNER_KEYWORD_TYPE_CONTINUE,
    },
    (keyword_translation_entry_t) {
        .string = "register",
        .length = 8,
        .keyword = SCANNER_KEYWORD_TYPE_REGISTER,
    },
    (keyword_translation_entry_t) {
        .string = "restrict",
        .length = 8,
        .keyword = SCANNER_KEYWORD_TYPE_RESTRICT,
    },
    (keyword_translation_entry_t) {
        .string = "unsigned",
        .length = 8,
        .keyword = SCANNER_KEYWORD_TYPE_UNSIGNED,
    },
    (keyword_translation_entry_t) {
        .string = "volatile",
        .length = 8,
        .keyword = SCANNER_KEYWORD_TYPE_VOLATILE,
    },
    (keyword_translation_entry_t) {
        .string = "default",
        .length = 7,
        .keyword = SCANNER_KEYWORD_TYPE_DEFAULT,
    },
    (keyword_translation_entry_t) {
        .string = "typedef",
        .length = 7,
        .keyword = SCANNER_KEYWORD_TYPE_TYPEDEF,
    },
    (keyword_translation_entry_t) {
        .string = "complex",
        .length = 7,
        .keyword = SCANNER_KEYWORD_TYPE_COMPLEX,
    },
    (keyword_translation_entry_t) {
        .string = "double",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_DOUBLE,
    },
    (keyword_translation_entry_t) {
        .string = "extern",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_EXTERN,
    },
    (keyword_translation_entry_t) {
        .string = "inline",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_INLINE,
    },
    (keyword_translation_entry_t) {
        .string = "return",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_RETURN,
    },
    (keyword_translation_entry_t) {
        .string = "signed",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_SIGNED,
    },
    (keyword_translation_entry_t) {
        .string = "sizeof",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_SIZEOF,
    },
    (keyword_translation_entry_t) {
        .string = "static",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_STATIC,
    },
    (keyword_translation_entry_t) {
        .string = "struct",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_STRUCT,
    },
    (keyword_translation_entry_t) {
        .string = "switch",
        .length = 6,
        .keyword = SCANNER_KEYWORD_TYPE_SWITCH,
    },
    (keyword_translation_entry_t) {
        .string = "break",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_BREAK,
    },
    (keyword_translation_entry_t) {
        .string = "const",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_CONST,
    },
    (keyword_translation_entry_t) {
        .string = "float",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_FLOAT,
    },
    (keyword_translation_entry_t) {
        .string = "short",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_SHORT,
    },
    (keyword_translation_entry_t) {
        .string = "union",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_UNION,
    },
    (keyword_translation_entry_t) {
        .string = "while",
        .length = 5,
        .keyword = SCANNER_KEYWORD_TYPE_WHILE,
    },
    (keyword_translation_entry_t) {
        .string = "case",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_CASE,
    },
    (keyword_translation_entry_t) {
        .string = "char",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_CHAR,
    },
    (keyword_translation_entry_t) {
        .string = "else",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_ELSE,
    },
    (keyword_translation_entry_t) {
        .string = "enum",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_ENUM,
    },
    (keyword_translation_entry_t) {
        .string = "goto",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_GOTO,
    },
    (keyword_translation_entry_t) {
        .string = "long",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_LONG,
    },
    (keyword_translation_entry_t) {
        .string = "void",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_VOID,
    },
    (keyword_translation_entry_t) {
        .string = "bool",
        .length = 4,
        .keyword = SCANNER_KEYWORD_TYPE_BOOL,
    },
    (keyword_translation_entry_t) {
        .string = "for",
        .length = 3,
        .keyword = SCANNER_KEYWORD_TYPE_FOR,
    },
    (keyword_translation_entry_t) {
        .string = "int",
        .length = 3,
        .keyword = SCANNER_KEYWORD_TYPE_INT,
    },
    (keyword_translation_entry_t) {
        .string = "do",
        .length = 2,
        .keyword = SCANNER_KEYWORD_TYPE_DO,
    },
    (keyword_translation_entry_t) {
        .string = "if",
        .length = 2,
        .keyword = SCANNER_KEYWORD_TYPE_IF,
    },
};

punctuation_translation_entry_t punctuation_translations[PUNCTUATION_TRANSLATIONS_COUNT] = {
    (punctuation_translation_entry_t) {
        .string = "<<=",
        .length = 3,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = ">>=",
        .length = 3,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "->*",
        .length = 3,
        .punctuation = SCANNER_PUNCTUATION_TYPE_POINTER_TO_DEREFERENCE,
    },
    (punctuation_translation_entry_t) {
        .string = "...",
        .length = 3,
        .punctuation = SCANNER_PUNCTUATION_TYPE_ELLIPSIS,
    },
    (punctuation_translation_entry_t) {
        .string = "++",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_INCREMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "--",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_DECREMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "==",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_EQUAL_TO,
    },
    (punctuation_translation_entry_t) {
        .string = "!=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_NOT_EQUAL_TO,
    },
    (punctuation_translation_entry_t) {
        .string = ">=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_GREATER_THAN_OR_EQUAL_TO,
    },
    (punctuation_translation_entry_t) {
        .string = "<=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_LESS_THAN_OR_EQUAL_TO,
    },
    (punctuation_translation_entry_t) {
        .string = "&&",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_LOGICAL_AND,
    },
    (punctuation_translation_entry_t) {
        .string = "||",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_LOGICAL_OR,
    },
    (punctuation_translation_entry_t) {
        .string = "|",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_OR,
    },
    (punctuation_translation_entry_t) {
        .string = "<<",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT,
    },
    (punctuation_translation_entry_t) {
        .string = ">>",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT,
    },
    (punctuation_translation_entry_t) {
        .string = "+=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_ADDITION_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "-=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_SUBTRACTION_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "*=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_MULTIPLICATION_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "/=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_DIVISION_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "%=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_MODULO_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "&=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_AND_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "|=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_OR_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "^=",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_XOR_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "->",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_DEREFERENCE,
    },
    (punctuation_translation_entry_t) {
        .string = ".*",
        .length = 2,
        .punctuation = SCANNER_PUNCTUATION_TYPE_POINTER_TO_REFERENCE,
    },
    (punctuation_translation_entry_t) {
        .string = "+",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_PLUS,
    },
    (punctuation_translation_entry_t) {
        .string = "-",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_MINUS,
    },
    (punctuation_translation_entry_t) {
        .string = "*",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_STAR,
    },
    (punctuation_translation_entry_t) {
        .string = "/",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_SLASH,
    },
    (punctuation_translation_entry_t) {
        .string = "%",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_PERCENT,
    },
    (punctuation_translation_entry_t) {
        .string = ">",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_GREATER_THAN,
    },
    (punctuation_translation_entry_t) {
        .string = "<",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_LESS_THAN,
    },
    (punctuation_translation_entry_t) {
        .string = "!",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_LOGICAL_NEGATION,
    },
    (punctuation_translation_entry_t) {
        .string = "~",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_NOT,
    },
    (punctuation_translation_entry_t) {
        .string = "&",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_AND,
    },
    (punctuation_translation_entry_t) {
        .string = "^",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_BITWISE_XOR,
    },
    (punctuation_translation_entry_t) {
        .string = "=",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT,
    },
    (punctuation_translation_entry_t) {
        .string = "{",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_CURLY_OPEN,
    },
    (punctuation_translation_entry_t) {
        .string = "}",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE,
    },
    (punctuation_translation_entry_t) {
        .string = "[",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_SQUARE_OPEN,
    },
    (punctuation_translation_entry_t) {
        .string = "]",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_SQUARE_CLOSE,
    },
    (punctuation_translation_entry_t) {
        .string = "(",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_PAREN_OPEN,
    },
    (punctuation_translation_entry_t) {
        .string = ")",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE,
    },
    (punctuation_translation_entry_t) {
        .string = ";",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_SEMICOLON,
    },
    (punctuation_translation_entry_t) {
        .string = ":",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_COLON,
    },
    (punctuation_translation_entry_t) {
        .string = "?",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_QUESTION_MARK,
    },
    (punctuation_translation_entry_t) {
        .string = ".",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_PERIOD,
    },
    (punctuation_translation_entry_t) {
        .string = ",",
        .length = 1,
        .punctuation = SCANNER_PUNCTUATION_TYPE_COMMA,
    },
};