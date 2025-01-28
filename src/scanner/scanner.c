#include <stdbool.h>

#include <debug/log.h>
#include <debug/error_handler.h>
#include <alloc.h>
#include <scanner/scanner.h>
#include <scanner/token_strings.h>

void scanner_init(scanner_t * scanner) {
    debug_printf("Initializing scanner\n");

    token_buffer_init(&scanner->tb);
}

void scanner_free(scanner_t * scanner) {
    debug_printf("Freeing scanner\n");

    token_buffer_free(&scanner->tb);
}

static inline bool is_symbol_char(char c) {
    return (
        (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        (c >= '0' && c <= '9') ||
        c == '_'
    );
}
static inline bool is_number_char(char c) {
    return (
        is_symbol_char(c) ||
        c == '.'
    );
}
static inline bool is_number_start_char(char c) {
    return (c >= '0' && c <= '9') || c == '-';
}
static inline bool is_number_postfix_char(char c) {
    return (
        (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z')
    );
}

void scanner_scan(scanner_t * scanner, const char * buffer) {
    debug_printf("Beginning scan\n");

    size_t position = 0;
    size_t line = 0;
    size_t line_start = 0;

    while (buffer[position] != '\0') {
        // remove unwanted characters
        if (buffer[position] == ' ' || buffer[position] == '\r') {
            debug_printf("Skipping character at position %zu\n", position);
            position++;
            continue;
        }
        else if (buffer[position] == '\n') {
            debug_printf("Got line feed at position %zu\n", position);
            position++;
            line++;
            line_start = position;
            continue;
        }

        if (is_number_start_char(buffer[position])) { // number
            debug_printf("Number\n");

            bool negative;
            if (buffer[position] == '-') {
                negative = true;
                position++;
            }
            else negative = false;

            bool is_float = false;
            size_t number_length;
            for (number_length = 0; is_number_char(buffer[position + number_length]); number_length++) {
                if (buffer[position + number_length] == '.') is_float = true;
            }

            size_t number_postfix_length;
            for (
                number_postfix_length = 0;
                is_number_postfix_char(buffer[position + number_length - number_postfix_length - 1]);
                number_postfix_length++
            );

            size_t number_start = 0;

            // check for number format
            int base;
            if (buffer[position] == '0') {
                number_start++;
                switch (buffer[position + 1]) {
                    case 'x': {
                        base = 16;
                        number_start++;
                    } break;
                    case 'b': {
                        base = 2;
                        number_start++;
                    } break;
                    default: {
                        base = 8;
                    } break;
                }
            }
            else base = 10;

            token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_KEYWORD);
            token_data_constant_t * data = token->data;

            token->line = line;
            token->position = position - line_start;

            // TODO: check for number type
            char * str_end;
            if (is_float) {
                data->type = SCANNER_CONSTANT_TYPE_DOUBLE;
                data->d = strtod(&buffer[position], &str_end);
            }
            else {
                if (negative) {
                    data->type = SCANNER_CONSTANT_TYPE_SLL;
                    data->s = strtoll(&buffer[position], &str_end, base);
                }
                else {
                    data->type = SCANNER_CONSTANT_TYPE_ULL;
                    data->u = strtoull(&buffer[position], &str_end, base);
                }
            }

            position += number_start + number_length + number_postfix_length;

            continue;
        }
        else if (buffer[position] == '"') { // string literal
            debug_printf("String\n");
        }
        else {
            // punctuation
            {
                bool punctuation_found = false;
                for (size_t i = 0; i < PUNCTUATION_STRINGS_COUNT; i++) {
                    if (strncmp(&buffer[position], punctuation_strings[i].string, punctuation_strings[i].length) == 0) {
                        debug_printf("Got punctuation '%s' at position %zu\n", punctuation_strings[i].string, position);

                        token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_KEYWORD);
                        token_data_punctuation_t * data = token->data;

                        token->line = line;
                        token->position = position - line_start;

                        data->type = punctuation_strings[i].punctuation;

                        position += punctuation_strings[i].length;

                        punctuation_found = true;
                        break;
                    }
                }
                if (punctuation_found) continue;
            }

            // keyword
            {
                bool keyword_found = false;
                for (size_t i = 0; i < KEYWORD_STRINGS_COUNT; i++) {
                    if (
                        strncmp(&buffer[position], keyword_strings[i].string, keyword_strings[i].length) == 0 &&
                        !is_symbol_char(buffer[position + keyword_strings[i].length])
                    ) {
                        debug_printf("Got keyword '%s' at position %zu\n", keyword_strings[i].string, position);

                        token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_KEYWORD);
                        token_data_keyword_t * data = token->data;

                        token->line = line;
                        token->position = position - line_start;

                        data->keyword = keyword_strings[i].keyword;

                        position += keyword_strings[i].length;

                        keyword_found = true;
                        break;
                    }
                }
                if (keyword_found) continue;
            }

            // identifier
            {
                size_t identifier_size = 0;

                while (is_symbol_char(buffer[position + identifier_size])) identifier_size++;

                if (identifier_size != 0) {
                    char * name = pkcc_alloc(identifier_size + 1);
                    memcpy(name, &buffer[position], identifier_size);
                    name[identifier_size] = '\0';

                    debug_printf("Got identifier '%s' at position %zu\n", name, position);

                    token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_KEYWORD);
                    token_data_identifier_t * data = token->data;

                    token->line = line;
                    token->position = position - line_start;
                    data->name = name;

                    position += identifier_size;

                    continue;
                }
            }
        }

        size_t line_length;
        for (line_length = 0; buffer[line_start + line_length] != '\n'; line_length++);

        fatal_error(
            "%.*s\n%*c\nEncountered unknown token at line %zu, character %zu\n",
            line_length, &buffer[line_start],
            position - line_start + 1, '^',
            line + 1,
            position - line_start + 1
        );
    }
}

void scanner_print_tokens(scanner_t * scanner) {
    
}