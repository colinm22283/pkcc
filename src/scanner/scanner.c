#include <stdbool.h>
#include <math.h>

#include <alloc.h>
#include <parsing/escape_characters.h>
#include <debug/log.h>
#include <debug/error_handler.h>
#include <scanner/scanner.h>
#include <scanner/token_translation.h>
#include <scanner/token_stringify.h>

void scanner_init(scanner_t * scanner) {
    log_printf("Initializing scanner\n");

    token_buffer_init(&scanner->tb);
}

void scanner_free(scanner_t * scanner) {
    log_printf("Freeing scanner\n");

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
        (c >= '0' && c <= '9') ||
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
    log_printf("Beginning scan\n");

    size_t position = 0;
    size_t line = 0;
    size_t line_start = 0;

    while (buffer[position] != '\0') {
        // remove unwanted characters
        if (buffer[position] == ' ' || buffer[position] == '\r') {
            log_printf("Skipping character at position %zu\n", position);
            position++;
            continue;
        }
        else if (buffer[position] == '\n') {
            log_printf("Got line feed at position %zu\n", position);
            position++;
            line++;
            line_start = position;
            continue;
        }

        if (is_number_start_char(buffer[position])) { // number
            size_t old_position = position;
            size_t number_start = 0;

            bool negative;
            if (buffer[position] == '-') {
                negative = true;
                position++;
                number_start++;
            }
            else negative = false;

            // check for number format
            int base;
            if (buffer[position] == '0') {
                position++;
                number_start++;
                switch (buffer[position + 1]) {
                    case 'x': {
                        base = 16;
                        position++;
                        number_start++;
                    } break;
                    case 'b': {
                        base = 2;
                        position++;
                        number_start++;
                    } break;
                    default: {
                        base = 8;
                    } break;
                }
            }
            else base = 10;

            bool is_float = false;
            size_t number_length;
            for (number_length = 0; is_number_char(buffer[position + number_length]); number_length++) {
                if (buffer[position + number_length] == '.') is_float = true;
            }

            size_t number_postfix_length;
            for (
                number_postfix_length = 0;
                is_number_postfix_char(buffer[position + number_length + number_postfix_length]);
                number_postfix_length++
            );

            token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_CONSTANT);
            token_data_constant_t * data = token->data;

            token->line = line;
            token->position = old_position - line_start;

            debug_printf("Number length: %u\n", number_length);

            char * str_end;
            if (is_float) {
                if (
                    number_postfix_length == 1 &&
                    buffer[position + number_length] == 'f'
                ) {
                    data->type = SCANNER_CONSTANT_TYPE_FLOAT;
                    data->f = strtof(&buffer[position], &str_end) * (negative ? -1.0f : 1.0f);
                }
                else {
                    data->type = SCANNER_CONSTANT_TYPE_DOUBLE;
                    data->d = strtod(&buffer[position], &str_end) * (negative ? -1.0 : 1.0);;
                }

                log_printf("Got number '%f' at position %zu\n", data->d, old_position);
            }
            else {
                if (negative) {
                    data->type = SCANNER_CONSTANT_TYPE_SLL;
                    data->s = strtoll(&buffer[position], &str_end, base);

                    log_printf("Got number '%lli' at position %zu\n", data->s, old_position);
                }
                else {
                    data->type = SCANNER_CONSTANT_TYPE_ULL;
                    data->u = strtoull(&buffer[position], &str_end, base);

                    log_printf("Got number '%llu' at position %zu\n", data->u, old_position);
                }
            }

            if (number_postfix_length != 0) {
                size_t clamped_postfix_length = number_postfix_length > 3 ? 3 : number_postfix_length;

                char postfix[4] = { '\0', '\0', '\0', '\0' };
                memcpy(postfix, &buffer[position + number_length - number_postfix_length], clamped_postfix_length);

                if (!is_float) {
                    size_t long_count = 0;
                    bool unsigned_present = false;

                    for (size_t i = 0; i < clamped_postfix_length; i++) {
                        if (postfix[i] == 'u' || postfix[i] == 'U') unsigned_present = true;
                        else if (postfix[i] == 'l' || postfix[i] == 'L') long_count++;
                        else {
                            size_t line_length;
                            for (line_length = 0; buffer[line_start + line_length] != '\n'; line_length++);
                            fatal_error(
                                "%.*s\n%*c\nEncountered invalid numerical suffix character at line %zu, character %zu\n",
                                line_length, &buffer[line_start],
                                old_position - line_start + number_length - number_postfix_length + i + 1, '^',
                                line + 1,
                                old_position - line_start + number_length - number_postfix_length + i + 1
                            );
                        }
                    }

                    if ((long_count == 0 && !unsigned_present) || long_count > 2) {
                        size_t line_length;
                        for (line_length = 0; buffer[line_start + line_length] != '\n'; line_length++);
                        fatal_error(
                            "%.*s\n%*c\nEncountered invalid numerical suffix at line %zu, character %zu\n",
                            line_length, &buffer[line_start],
                            old_position - line_start + number_length - number_postfix_length + 1, '^',
                            line + 1,
                            old_position - line_start + number_length - number_postfix_length + 1
                        );
                    }

                    if (unsigned_present) {
                        switch (long_count) {
                            case 0: {
                                data->type = SCANNER_CONSTANT_TYPE_UI;
                            } break;
                            case 1: {
                                data->type = SCANNER_CONSTANT_TYPE_UL;
                            } break;
                            case 2: {
                                data->type = SCANNER_CONSTANT_TYPE_ULL;
                            } break;

                            default: break;
                        }
                    }
                }
            }

            position += number_start + number_length + number_postfix_length;

            continue;
        }
        else if (buffer[position] == '"') { // string literal
            size_t string_size;
            for (
                string_size = 0;
                buffer[position + string_size + 1] != '"' ||
                buffer[position + string_size] == '\\';
                string_size++
            );

            token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_STRING_LITERAL);
            token_data_string_literal_t * data = token->data;

            token->line = line;
            token->position = position - line_start;

            data->content = pkcc_alloc(string_size + 1);
            memcpy(data->content, &buffer[position + 1], string_size);
            data->content[string_size] = '\0';

            log_printf("Got string '%s' at position %zu\n", data->content, position);

            position += string_size + 2;

            continue;
        }
        else if (buffer[position] == '\'') { // character literal
            if (buffer[position + 1] == '\'') {
                size_t line_length;
                for (line_length = 0; buffer[line_start + line_length] != '\n'; line_length++);
                fatal_error(
                    "%.*s\n%*c\nEncountered empty character literal at line %zu, character %zu\n",
                    line_length, &buffer[line_start],
                    position - line_start + 2, '^',
                    line + 1,
                    position - line_start + 2
                );
            }

            if (buffer[position + 1] == '\\') {
                size_t escape_size;
                char escape_result = parse_escape_char(&buffer[position + 2], &escape_size);

                if (escape_size != 0) {
                    if (buffer[position + escape_size + 2] != '\'') {
                        size_t line_length;
                        for (line_length = 0; buffer[line_start + line_length] != '\n'; line_length++);
                        fatal_error(
                            "%.*s\n%*c\nEncountered invalid character literal at line %zu, character %zu\n",
                            line_length, &buffer[line_start],
                            position - line_start + 1, '^',
                            line + 1,
                            position - line_start + 1
                        );
                    }

                    log_printf("Escaped character\n");

                    token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_CONSTANT);
                    token_data_constant_t * data = token->data;

                    token->line = line;
                    token->position = position - line_start;

                    data->type = SCANNER_CONSTANT_TYPE_SC;
                    data->s = (long long) escape_result;

                    position += 3 + escape_size;

                    continue;
                }
                else {
                    size_t line_length;
                    for (line_length = 0; buffer[line_start + line_length] != '\n'; line_length++);
                    fatal_error(
                        "%.*s\n%*c\nEncountered invalid character literal at line %zu, character %zu\n",
                        line_length, &buffer[line_start],
                        position - line_start + 1, '^',
                        line + 1,
                        position - line_start + 1
                    );
                }
            }

            if (buffer[position + 2] != '\''){
                size_t line_length;
                for (line_length = 0; buffer[line_start + line_length] != '\n'; line_length++);
                fatal_error(
                    "%.*s\n%*c\nEncountered invalid character literal at line %zu, character %zu\n",
                    line_length, &buffer[line_start],
                    position - line_start + 1, '^',
                    line + 1,
                    position - line_start + 1
                );
            }

            log_printf("Standard character\n");

            token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_CONSTANT);
            token_data_constant_t * data = token->data;

            token->line = line;
            token->position = position - line_start;

            data->type = SCANNER_CONSTANT_TYPE_SC;
            data->s = (long long) buffer[position + 1];

            position += 3;

            continue;
        }
        else {
            // punctuation
            {
                bool punctuation_found = false;
                for (size_t i = 0; i < PUNCTUATION_TRANSLATIONS_COUNT; i++) {
                    if (strncmp(&buffer[position], punctuation_translations[i].string, punctuation_translations[i].length) == 0) {
                        log_printf("Got punctuation '%s' at position %zu\n", punctuation_translations[i].string, position);

                        token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_PUNCTUATION);
                        token_data_punctuation_t * data = token->data;

                        token->line = line;
                        token->position = position - line_start;

                        data->type = punctuation_translations[i].punctuation;

                        position += punctuation_translations[i].length;

                        punctuation_found = true;
                        break;
                    }
                }

                if (punctuation_found) continue;
            }

            // keyword
            {
                bool keyword_found = false;
                for (size_t i = 0; i < KEYWORD_TRANSLATIONS_COUNT; i++) {
                    if (
                        strncmp(&buffer[position], keyword_translations[i].string, keyword_translations[i].length) == 0 &&
                        !is_symbol_char(buffer[position + keyword_translations[i].length])
                    ) {
                        log_printf("Got keyword '%s' at position %zu\n", keyword_translations[i].string, position);

                        token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_KEYWORD);
                        token_data_keyword_t * data = token->data;

                        token->line = line;
                        token->position = position - line_start;

                        data->keyword = keyword_translations[i].keyword;

                        position += keyword_translations[i].length;

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

                    log_printf("Got identifier '%s' at position %zu\n", name, position);

                    token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_IDENTIFIER);
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
    char str_buffer[TOKEN_STRINGIFY_BUFFER_REQUIREMENT];

    size_t line = 0;

    log_printf("%-6s Tokens\n", "Line");
    log_printf("%-6zu ", 1);

    for (size_t i = 0; i < scanner->tb.token_count; i++) {
        token_t * token = &scanner->tb.tokens[i];

        if (line != token->line) {
            line = token->line;

            log_printf("\n%-6zu ", token->line + 1);
            fflush(stdout);
        }

        token_stringify(str_buffer, token);
        log_printf("%s ", str_buffer);
        fflush(stdout);
    }

    log_printf("\n");
}