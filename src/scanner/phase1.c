#include <scanner/phase1.h>
#include <scanner/token.h>
#include <scanner/token_stringify.h>

size_t phase1_keywords[] = {
    [SCANNER_KEYWORD_TYPE_BREAK] = 408,
    [SCANNER_KEYWORD_TYPE_CASE] = 412,
    [SCANNER_KEYWORD_TYPE_CHAR] = 301,
    [SCANNER_KEYWORD_TYPE_CONST] = 401,
    [SCANNER_KEYWORD_TYPE_CONTINUE] = 409,
    [SCANNER_KEYWORD_TYPE_DEFAULT] = 413,
    [SCANNER_KEYWORD_TYPE_DO] = 405,
    [SCANNER_KEYWORD_TYPE_DOUBLE] = 301,
    [SCANNER_KEYWORD_TYPE_ELSE] = 407,
    [SCANNER_KEYWORD_TYPE_ENUM] = 414,
    [SCANNER_KEYWORD_TYPE_EXTERN] = 415,
    [SCANNER_KEYWORD_TYPE_FLOAT] = 301,
    [SCANNER_KEYWORD_TYPE_FOR] = 403,
    [SCANNER_KEYWORD_TYPE_GOTO] = 416,
    [SCANNER_KEYWORD_TYPE_IF] = 406,
    [SCANNER_KEYWORD_TYPE_INLINE] = 417,
    [SCANNER_KEYWORD_TYPE_INT] = 301,
    [SCANNER_KEYWORD_TYPE_LONG] = 301,
    [SCANNER_KEYWORD_TYPE_REGISTER] = 418,
    [SCANNER_KEYWORD_TYPE_RESTRICT] = 419,
    [SCANNER_KEYWORD_TYPE_RETURN] = 410,
    [SCANNER_KEYWORD_TYPE_SHORT] = 301,
    [SCANNER_KEYWORD_TYPE_SIGNED] = 420,
    [SCANNER_KEYWORD_TYPE_SIZEOF] = 421,
    [SCANNER_KEYWORD_TYPE_STATIC] = 422,
    [SCANNER_KEYWORD_TYPE_STRUCT] = 402,
    [SCANNER_KEYWORD_TYPE_SWITCH] = 411,
    [SCANNER_KEYWORD_TYPE_TYPEDEF] = 423,
    [SCANNER_KEYWORD_TYPE_UNION] = 424,
    [SCANNER_KEYWORD_TYPE_UNSIGNED] = 425,
    [SCANNER_KEYWORD_TYPE_VOID] = 301,
    [SCANNER_KEYWORD_TYPE_VOLATILE] = 426,
    [SCANNER_KEYWORD_TYPE_WHILE] = 404,
    [SCANNER_KEYWORD_TYPE_BOOL] = 301,
    [SCANNER_KEYWORD_TYPE_COMPLEX] = 427,
    [SCANNER_KEYWORD_TYPE_IMAGINARY] = 328,
};

size_t phase1_constants[] = {
    [SCANNER_CONSTANT_TYPE_UC] = 302,
    [SCANNER_CONSTANT_TYPE_US] = 302,
    [SCANNER_CONSTANT_TYPE_UI] = 303,
    [SCANNER_CONSTANT_TYPE_UL] = 303,
    [SCANNER_CONSTANT_TYPE_ULL] = 303,

    [SCANNER_CONSTANT_TYPE_SC] = 302,
    [SCANNER_CONSTANT_TYPE_SS] = 302,
    [SCANNER_CONSTANT_TYPE_SI] = 303,
    [SCANNER_CONSTANT_TYPE_SL] = 303,
    [SCANNER_CONSTANT_TYPE_SLL] = 303,

    [SCANNER_CONSTANT_TYPE_FLOAT] = 304,
    [SCANNER_CONSTANT_TYPE_DOUBLE] = 304,
};

size_t phase1_punctuation[] = {
    [SCANNER_PUNCTUATION_TYPE_PLUS] = 42,
    [SCANNER_PUNCTUATION_TYPE_MINUS] = 45,
    [SCANNER_PUNCTUATION_TYPE_STAR] = 42,
    [SCANNER_PUNCTUATION_TYPE_SLASH] = 47,
    [SCANNER_PUNCTUATION_TYPE_PERCENT] = 37,
    [SCANNER_PUNCTUATION_TYPE_INCREMENT] = 355,
    [SCANNER_PUNCTUATION_TYPE_DECREMENT] = 356,

    [SCANNER_PUNCTUATION_TYPE_EQUAL_TO] = 351,
    [SCANNER_PUNCTUATION_TYPE_NOT_EQUAL_TO] = 352,
    [SCANNER_PUNCTUATION_TYPE_GREATER_THAN] = 62,
    [SCANNER_PUNCTUATION_TYPE_LESS_THAN] = 60,
    [SCANNER_PUNCTUATION_TYPE_GREATER_THAN_OR_EQUAL_TO] = 353,
    [SCANNER_PUNCTUATION_TYPE_LESS_THAN_OR_EQUAL_TO] = 354,

    [SCANNER_PUNCTUATION_TYPE_LOGICAL_NEGATION] = 33,
    [SCANNER_PUNCTUATION_TYPE_LOGICAL_AND] = 358,
    [SCANNER_PUNCTUATION_TYPE_LOGICAL_OR] = 357,

    [SCANNER_PUNCTUATION_TYPE_BITWISE_NOT] = 126,
    [SCANNER_PUNCTUATION_TYPE_AND] = 38,
    [SCANNER_PUNCTUATION_TYPE_BITWISE_OR] = 124,
    [SCANNER_PUNCTUATION_TYPE_BITWISE_XOR] = 94,
    [SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT] = 365,
    [SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT] = 366,

    [SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT] = 61,
    [SCANNER_PUNCTUATION_TYPE_ADDITION_ASSIGNMENT] = 361,
    [SCANNER_PUNCTUATION_TYPE_SUBTRACTION_ASSIGNMENT] = 362,
    [SCANNER_PUNCTUATION_TYPE_MULTIPLICATION_ASSIGNMENT] = 363,
    [SCANNER_PUNCTUATION_TYPE_DIVISION_ASSIGNMENT] = 364,
    [SCANNER_PUNCTUATION_TYPE_MODULO_ASSIGNMENT] = 367,
    [SCANNER_PUNCTUATION_TYPE_BITWISE_AND_ASSIGNMENT] = 368,
    [SCANNER_PUNCTUATION_TYPE_BITWISE_OR_ASSIGNMENT] = 369,
    [SCANNER_PUNCTUATION_TYPE_BITWISE_XOR_ASSIGNMENT] = 370,
    [SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT_ASSIGNMENT] = 371,
    [SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT_ASSIGNMENT] = 372,

    [SCANNER_PUNCTUATION_TYPE_DEREFERENCE] = 373,
    [SCANNER_PUNCTUATION_TYPE_POINTER_TO_DEREFERENCE] = 374,
    [SCANNER_PUNCTUATION_TYPE_POINTER_TO_REFERENCE] = 375,

    [SCANNER_PUNCTUATION_TYPE_CURLY_OPEN] = 123,
    [SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE] = 125,
    [SCANNER_PUNCTUATION_TYPE_SQUARE_OPEN] = 91,
    [SCANNER_PUNCTUATION_TYPE_SQUARE_CLOSE] = 93,
    [SCANNER_PUNCTUATION_TYPE_PAREN_OPEN] = 40,
    [SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE] = 41,

    [SCANNER_PUNCTUATION_TYPE_SEMICOLON] = 59,
    [SCANNER_PUNCTUATION_TYPE_COLON] = 58,

    [SCANNER_PUNCTUATION_TYPE_ELLIPSIS] = 376,
    [SCANNER_PUNCTUATION_TYPE_QUESTION_MARK] = 63,
    [SCANNER_PUNCTUATION_TYPE_PERIOD] = 46,
    [SCANNER_PUNCTUATION_TYPE_COMMA] = 44,
};

void scanner_print_phase1(FILE * out_stream, scanner_t * scanner) {
    for (size_t i = 0; i < scanner->tb.token_count; i++) {
        token_t * token = &scanner->tb.tokens[i];

        fprintf(out_stream, "File '%s' Line %zu Token ", token->file_name->absolute_path, token->line_index + 1);

        switch (token->type) {
            case TOKEN_TYPE_IDENTIFIER: {
                token_data_identifier_t * identifier = (token_data_identifier_t *) token->data;

                fprintf(out_stream, "306 Text %s\n", identifier->name);
            } break;

            case TOKEN_TYPE_KEYWORD: {
                token_data_keyword_t * keyword = (token_data_keyword_t *) token->data;

                fprintf(out_stream, "%zu Text %s\n", phase1_keywords[keyword->keyword], scanner_stringify_phase1(token));
            } break;

            case TOKEN_TYPE_CONSTANT: {
                token_data_constant_t * constant = (token_data_constant_t *) token->data;

                switch (constant->type) {
                    case SCANNER_CONSTANT_TYPE_FLOAT:
                        fprintf(out_stream, "%zu Text %f\n", phase1_constants[constant->type], constant->f);
                        break;

                    case SCANNER_CONSTANT_TYPE_DOUBLE:
                        fprintf(out_stream, "%zu Text %f\n", phase1_constants[constant->type], constant->d);
                        break;

                    case SCANNER_CONSTANT_TYPE_UC:
                    case SCANNER_CONSTANT_TYPE_SC:
                        fprintf(out_stream, "%zu Text '%c'\n", phase1_constants[constant->type], (char) constant->i);
                        break;

                    case SCANNER_CONSTANT_TYPE_US:
                    case SCANNER_CONSTANT_TYPE_UI:
                    case SCANNER_CONSTANT_TYPE_UL:
                    case SCANNER_CONSTANT_TYPE_ULL:
                    case SCANNER_CONSTANT_TYPE_SS:
                    case SCANNER_CONSTANT_TYPE_SI:
                    case SCANNER_CONSTANT_TYPE_SL:
                    case SCANNER_CONSTANT_TYPE_SLL:
                        fprintf(out_stream, "%zu Text %llu\n", phase1_constants[constant->type], constant->i);
                        break;
                }
            } break;

            case TOKEN_TYPE_PUNCTUATION: {
                token_data_punctuation_t * punctuation = (token_data_punctuation_t *) token->data;

                fprintf(out_stream, "%zu Text %s\n", phase1_punctuation[punctuation->type], scanner_stringify_phase1(token));
            } break;

            case TOKEN_TYPE_STRING_LITERAL: {
                token_data_string_literal_t * string_literal = (token_data_string_literal_t *) token->data;

                fprintf(out_stream, "305 Text \"%s\"\n", string_literal->content);
            } break;
        }
    }
}

const char * scanner_stringify_phase1(token_t * token) {
    switch (token->type) {
        case TOKEN_TYPE_KEYWORD: {
            token_data_keyword_t * keyword = (token_data_keyword_t *) token->data;

            switch (keyword->keyword) {
                case SCANNER_KEYWORD_TYPE_BREAK: return "break";
                case SCANNER_KEYWORD_TYPE_CASE: return "case";
                case SCANNER_KEYWORD_TYPE_CHAR: return "char";
                case SCANNER_KEYWORD_TYPE_CONST: return "const";
                case SCANNER_KEYWORD_TYPE_CONTINUE: return "continue";
                case SCANNER_KEYWORD_TYPE_DEFAULT: return "default";
                case SCANNER_KEYWORD_TYPE_DO: return "do";
                case SCANNER_KEYWORD_TYPE_DOUBLE: return "double";
                case SCANNER_KEYWORD_TYPE_ELSE: return "else";
                case SCANNER_KEYWORD_TYPE_ENUM: return "enum";
                case SCANNER_KEYWORD_TYPE_EXTERN: return "extern";
                case SCANNER_KEYWORD_TYPE_FLOAT: return "float";
                case SCANNER_KEYWORD_TYPE_FOR: return "for";
                case SCANNER_KEYWORD_TYPE_GOTO: return "goto";
                case SCANNER_KEYWORD_TYPE_IF: return "if";
                case SCANNER_KEYWORD_TYPE_INLINE: return "inline";
                case SCANNER_KEYWORD_TYPE_INT: return "int";
                case SCANNER_KEYWORD_TYPE_LONG: return "long";
                case SCANNER_KEYWORD_TYPE_REGISTER: return "register";
                case SCANNER_KEYWORD_TYPE_RESTRICT: return "restrict";
                case SCANNER_KEYWORD_TYPE_RETURN: return "return";
                case SCANNER_KEYWORD_TYPE_SHORT: return "short";
                case SCANNER_KEYWORD_TYPE_SIGNED: return "signed";
                case SCANNER_KEYWORD_TYPE_SIZEOF: return "sizeof";
                case SCANNER_KEYWORD_TYPE_STATIC: return "static";
                case SCANNER_KEYWORD_TYPE_STRUCT: return "struct";
                case SCANNER_KEYWORD_TYPE_SWITCH: return "switch";
                case SCANNER_KEYWORD_TYPE_TYPEDEF: return "typedef";
                case SCANNER_KEYWORD_TYPE_UNION: return "union";
                case SCANNER_KEYWORD_TYPE_UNSIGNED: return "unsigned";
                case SCANNER_KEYWORD_TYPE_VOID: return "void";
                case SCANNER_KEYWORD_TYPE_VOLATILE: return "volatile";
                case SCANNER_KEYWORD_TYPE_WHILE: return "while";
                case SCANNER_KEYWORD_TYPE_BOOL: return "_Bool";
                case SCANNER_KEYWORD_TYPE_COMPLEX: return "complex";
                case SCANNER_KEYWORD_TYPE_IMAGINARY: return "imaginary";

                default: return "UNKNOWN";
            }
        }

        case TOKEN_TYPE_PUNCTUATION: {
            token_data_punctuation_t * punctuation = (token_data_punctuation_t *) token->data;

            switch (punctuation->type) {
                case SCANNER_PUNCTUATION_TYPE_PLUS: return "+";
                case SCANNER_PUNCTUATION_TYPE_MINUS: return "-";
                case SCANNER_PUNCTUATION_TYPE_STAR: return "*";
                case SCANNER_PUNCTUATION_TYPE_SLASH: return "/";
                case SCANNER_PUNCTUATION_TYPE_PERCENT: return "%";
                case SCANNER_PUNCTUATION_TYPE_INCREMENT: return "++";
                case SCANNER_PUNCTUATION_TYPE_DECREMENT: return "--";

                case SCANNER_PUNCTUATION_TYPE_EQUAL_TO: return "==";
                case SCANNER_PUNCTUATION_TYPE_NOT_EQUAL_TO: return "!=";
                case SCANNER_PUNCTUATION_TYPE_GREATER_THAN: return ">";
                case SCANNER_PUNCTUATION_TYPE_LESS_THAN: return "<";
                case SCANNER_PUNCTUATION_TYPE_GREATER_THAN_OR_EQUAL_TO: return ">=";
                case SCANNER_PUNCTUATION_TYPE_LESS_THAN_OR_EQUAL_TO: return "<=";

                case SCANNER_PUNCTUATION_TYPE_LOGICAL_NEGATION: return "!";
                case SCANNER_PUNCTUATION_TYPE_LOGICAL_AND: return "&&";
                case SCANNER_PUNCTUATION_TYPE_LOGICAL_OR: return "||";

                case SCANNER_PUNCTUATION_TYPE_BITWISE_NOT: return "~";
                case SCANNER_PUNCTUATION_TYPE_AND: return "&";
                case SCANNER_PUNCTUATION_TYPE_BITWISE_OR: return "|";
                case SCANNER_PUNCTUATION_TYPE_BITWISE_XOR: return "^";
                case SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT: return "<<";
                case SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT: return ">>";

                case SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT: return "=";
                case SCANNER_PUNCTUATION_TYPE_ADDITION_ASSIGNMENT: return "+=";
                case SCANNER_PUNCTUATION_TYPE_SUBTRACTION_ASSIGNMENT: return "-=";
                case SCANNER_PUNCTUATION_TYPE_MULTIPLICATION_ASSIGNMENT: return "*=";
                case SCANNER_PUNCTUATION_TYPE_DIVISION_ASSIGNMENT: return "/=";
                case SCANNER_PUNCTUATION_TYPE_MODULO_ASSIGNMENT: return "%=";
                case SCANNER_PUNCTUATION_TYPE_BITWISE_AND_ASSIGNMENT: return "&=";
                case SCANNER_PUNCTUATION_TYPE_BITWISE_OR_ASSIGNMENT: return "|=";
                case SCANNER_PUNCTUATION_TYPE_BITWISE_XOR_ASSIGNMENT: return "^=";
                case SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_LEFT_ASSIGNMENT: return "<<=";
                case SCANNER_PUNCTUATION_TYPE_BITWISE_SHIFT_RIGHT_ASSIGNMENT: return ">>=";

                case SCANNER_PUNCTUATION_TYPE_DEREFERENCE: return "->";
                case SCANNER_PUNCTUATION_TYPE_POINTER_TO_DEREFERENCE: return "->*";
                case SCANNER_PUNCTUATION_TYPE_POINTER_TO_REFERENCE: return ".*";

                case SCANNER_PUNCTUATION_TYPE_CURLY_OPEN: return "{";
                case SCANNER_PUNCTUATION_TYPE_CURLY_CLOSE: return "}";
                case SCANNER_PUNCTUATION_TYPE_SQUARE_OPEN: return "[";
                case SCANNER_PUNCTUATION_TYPE_SQUARE_CLOSE: return "]";
                case SCANNER_PUNCTUATION_TYPE_PAREN_OPEN: return "(";
                case SCANNER_PUNCTUATION_TYPE_PAREN_CLOSE: return ")";

                case SCANNER_PUNCTUATION_TYPE_SEMICOLON: return ";";
                case SCANNER_PUNCTUATION_TYPE_COLON: return ":";

                case SCANNER_PUNCTUATION_TYPE_ELLIPSIS: return "...";
                case SCANNER_PUNCTUATION_TYPE_QUESTION_MARK: return "?";
                case SCANNER_PUNCTUATION_TYPE_PERIOD: return ".";
                case SCANNER_PUNCTUATION_TYPE_COMMA: return ",";

                default: return "UNKNOWN";
            }
        }

        default: return NULL;
    }
}