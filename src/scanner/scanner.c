#include <stdbool.h>

#include <alloc.h>
#include <parsing/escape_characters.h>
#include <parsing/is_symbol_char.h>
#include <parsing/is_whitespace.h>
#include <parsing/is_number_char.h>
#include <debug/log.h>
#include <debug/line_error.h>
#include <debug/line_warning.h>
#include <scanner/scanner.h>
#include <scanner/token_translation.h>
#include <scanner/token_stringify.h>
#include <scanner/parse_number.h>

void scanner_init(scanner_t * scanner) {
    log_printf("Initializing scanner\n");

    token_buffer_init(&scanner->tb);
}

void scanner_free(scanner_t * scanner) {
    log_printf("Freeing scanner\n");

    token_buffer_free(&scanner->tb);
}

static inline bool is_number_postfix_char(char c) {
    return (
        (c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z')
    );
}

void scanner_scan(scanner_t * scanner, line_buffer_t * line_buffer) {
    log_printf("Beginning scan\n");

    for (size_t line_index = 0; line_index < line_buffer->size; line_index++) {
        line_buffer_line_t * line = &line_buffer->lines[line_index];

        size_t line_number = line->metadata.source_line;
        file_name_entry_t * file_name = line->metadata.file_name;

        size_t position = 0;
        const char * buffer = line->line;

        while (buffer[position] != '\0') {
            log_printf("Parsing section '%s'\n", &buffer[position]);

            {
                size_t whitespace_skip = 0;
                while (is_whitespace(buffer[position + whitespace_skip])) whitespace_skip++;
                if (whitespace_skip > 0) log_printf("Skipped %zu character(s) of whitespace\n", whitespace_skip);

                position += whitespace_skip;
            }

            if (is_number_char(buffer[position])) { // number
                position = scanner_parse_number(&scanner->tb, file_name, line_buffer, line_index, line_number, position, NULL);

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

                token->file_name = line->metadata.file_name;
                token->line = line_number;
                token->position = position;

                data->content = pkcc_alloc(string_size + 1);
                memcpy(data->content, &buffer[position + 1], string_size);
                data->content[string_size] = '\0';

                log_printf("Got string '%s' at position %zu\n", data->content, position);

                if (options.max_string_length != 0 && string_size > options.max_string_length) {
                    line_range_warning(
                        line_buffer,
                        file_name,
                        "Excess string length (see -fmax-string-literal-length)",
                        line_index,
                        position,
                        string_size + 2
                    );
                }

                position += string_size + 2;

                continue;
            }
            else if (buffer[position] == '\'') { // character literal
                if (buffer[position + 1] == '\'') {
                    fatal_line_error(
                        line_buffer,
                        file_name,
                        "Encountered empty character literal",
                        line_index,
                        position
                    );
                }

                if (buffer[position + 1] == '\\') {
                    size_t escape_size;
                    char escape_result = parse_escape_char(&buffer[position + 2], &escape_size);

                    if (escape_size != 0) {
                        if (buffer[position + escape_size + 2] != '\'') {
                            fatal_line_error(
                                line_buffer,
                                file_name,
                                "Encountered empty character literal",
                                line_index,
                                position
                            );
                        }

                        log_printf("Escaped character\n");

                        token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_CONSTANT);
                        token_data_constant_t * data = token->data;

                        token->file_name = line->metadata.file_name;
                        token->line = line_number;
                        token->position = position;

                        data->type = SCANNER_CONSTANT_TYPE_SC;
                        data->i = (unsigned long long) escape_result;

                        position += 3 + escape_size;

                        continue;
                    }
                    else {
                        fatal_line_error(
                            line_buffer,
                            file_name,
                            "Encountered invalid character literal",
                            line_index,
                            position
                        );
                    }
                }

                if (buffer[position + 2] != '\'') {
                    fatal_line_error(
                        line_buffer,
                        file_name,
                        "Encountered invalid character literal",
                        line_index,
                        position
                    );
                }

                log_printf("Standard character\n");

                token_t * token = token_buffer_push(&scanner->tb, TOKEN_TYPE_CONSTANT);
                token_data_constant_t * data = token->data;

                token->file_name = line->metadata.file_name;
                token->line = line_number;
                token->position = position;

                data->type = SCANNER_CONSTANT_TYPE_SC;
                data->i = (unsigned long long) buffer[position + 1];

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

                            token->file_name = line->metadata.file_name;
                            token->line = line_number;
                            token->position = position;

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

                            token->file_name = line->metadata.file_name;
                            token->line = line_number;
                            token->position = position;

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

                        token->file_name = line->metadata.file_name;
                        token->line = line_number;
                        token->position = position;
                        data->name = name;

                        position += identifier_size;

                        continue;
                    }
                }
            }

            fatal_line_error(
                line_buffer,
                file_name,
                "Encountered unknown token",
                line_index,
                position
            );
        }
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