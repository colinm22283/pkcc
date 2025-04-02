#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

#include <alloc.h>

#include <file/file_loader.h>
#include <file/file_exists.h>
#include <preprocessor/preprocessor.h>
#include <parsing/is_whitespace.h>
#include <parsing/is_symbol_char.h>
#include <parsing/is_line_end.h>

void preprocessor_init(preprocessor_t * preprocessor, file_name_registry_t * fnr, const char * absolute_path, const char * include_path, size_t depth) {
    log_printf("Initializing preprocessor\n");

    preprocessor->current_depth = depth;

    preprocessor->fnr = fnr;
    preprocessor->file_name = file_name_registry_push(fnr, absolute_path, include_path);

    preprocessor_token_buffer_init(&preprocessor->tbuf);
    preprocessor_vmap_init(&preprocessor->vmap);

    preprocessor_comment_filter_init(&preprocessor->cfilter);
}

void preprocessor_free(preprocessor_t * preprocessor) {
    log_printf("Freeing preprocessor\n");

    preprocessor_comment_filter_free(&preprocessor->cfilter);

    preprocessor_vmap_free(&preprocessor->vmap);
    preprocessor_token_buffer_free(&preprocessor->tbuf);
}

void preprocessor_parse(preprocessor_t * preprocessor, const char * input_data) {
    preprocessor_comment_filter_parse(&preprocessor->cfilter, input_data);

    const char * data = preprocessor->cfilter.data;

    size_t position = 0;
    size_t line = 0;
    while (true) {
        size_t line_start = position;

        if (data[position] == '\0') break;
        else if (data[position] == '#') {
            log_printf("Got preprocessor directive on line %zu\n", line + 1);

            position++;
            while (data[position] == ' ') position++;

            if (strncmp(&data[position], "include", 7) == 0) {
                position += 7;

                while (is_whitespace(data[position])) position++;

                char start_char = data[position];
                char end_char = 0;
                bool user_include;

                if (start_char == '<') {
                    user_include = false;
                    end_char = '>';
                }
                else if (start_char == '"') {
                    user_include = true;
                    end_char = '"';
                }
                else {
                    size_t line_length;
                    for (line_length = 0; data[line_start + line_length] != '\n'; line_length++);
                    fatal_error(
                        "%.*s\n%*c\nInvalid include directive opening char, use \" or < at line %zu, character %zu\n",
                        line_length, &data[line_start],
                        8, '^',
                        line + 1,
                        8
                    );
                    return;
                }
                position++;

                const char * path_ptr = &data[position];
                size_t path_size = 0;
                while (data[position + path_size] != end_char) {
                    if (is_line_end(data[position + path_size])) {
                        size_t line_length;
                        for (line_length = 0; data[line_start + line_length] != '\n'; line_length++);
                        fatal_error(
                            "%.*s\n%*c\nUnclosed include directive path argument at line %zu, character %zu\n",
                            line_length, &data[line_start],
                            position + path_size, '^',
                            line + 1,
                            position + path_size
                        );
                    }

                    path_size++;
                }

                char * path = pkcc_alloc(path_size + 1);
                memcpy(path, path_ptr, path_size);
                path[path_size] = '\0';

                log_printf(
                    "Loading %s path include file from \"%s\"\n",
                    user_include ? "user" : "library",
                    path
                );

                char * resolved_path = NULL;
                if (user_include) {
                    if (!file_exists(path)) {
                        pkcc_free(path);

                        size_t line_length;
                        for (line_length = 0; data[line_start + line_length] != '\n'; line_length++);
                        fatal_error(
                            "%.*s\n%*c\nInclude file not found from line %zu, character %zu\n",
                            line_length, &data[line_start],
                            position + path_size, '^',
                            line + 1,
                            position + path_size
                        );
                    }

                    resolved_path = path;
                }
                else {
                    for (size_t i = 0; i < options.include_directories_size; i++) {
                        size_t include_dir_size = strlen(options.include_directories[i]);

                        resolved_path = pkcc_alloc(include_dir_size + path_size + 2);
                        memcpy(resolved_path, options.include_directories[i], include_dir_size);
                        resolved_path[include_dir_size] = '/';
                        strcpy(&resolved_path[include_dir_size + 1], path);

                        if (file_exists(resolved_path)) {
                            pkcc_free(path);
                            path = NULL;
                            break;
                        }
                    }

                    if (path != NULL) {
                        pkcc_free(path);

                        size_t line_length;
                        for (line_length = 0; data[line_start + line_length] != '\n'; line_length++);
                        fatal_error(
                            "%.*s\n%*c\nInclude file not found from line %zu, character %zu\n",
                            line_length, &data[line_start],
                            position + path_size, '^',
                            line + 1,
                            position + path_size
                        );
                    }
                }

                file_loader_t file_loader;
                file_loader_init(&file_loader, resolved_path);

                char absolute_path_buffer[PATH_MAX];
                const char * absolute_path = realpath(resolved_path, absolute_path_buffer);

                log_printf("Include file absolute path: '%s'\n", absolute_path);

                preprocessor_t sub_preprocessor;
                preprocessor_init(&sub_preprocessor, preprocessor->fnr, absolute_path, resolved_path, preprocessor->current_depth + 1);

                if (sub_preprocessor.current_depth == options.max_preprocessor_depth) {
                    size_t line_length;
                    for (line_length = 0; data[line_start + line_length] != '\n' && data[line_start + line_length] != '\0'; line_length++);
                    fatal_error(
                        "%.*s\n%*c\nUnknown preprocessor directive at line %zu, character %zu\n",
                        line_length, &data[line_start],
                        2, '^',
                        line + 1,
                        2
                    );
                }

                preprocessor_parse(&sub_preprocessor, file_loader.data);

                for (size_t i = 0; i < sub_preprocessor.tbuf.size; i++) {
                    preprocessor_token_t * new_token = preprocessor_token_buffer_push(&preprocessor->tbuf);

                    if (sub_preprocessor.tbuf.tokens[i].file_name == NULL) fatal_error("oh dear\n");

                    preprocessor_token_buffer_token_clone(new_token, &sub_preprocessor.tbuf.tokens[i]);
                }

                preprocessor_free(&sub_preprocessor);
                file_loader_free(&file_loader);
                pkcc_free(resolved_path);
            }
            else {
                size_t line_length;
                for (line_length = 0; data[line_start + line_length] != '\n'; line_length++);
                fatal_error(
                    "%.*s\n%*c\nUnknown preprocessor directive at line %zu, character %zu\n",
                    line_length, &data[line_start],
                    2, '^',
                    line + 1,
                    2
                );
            }

            while (data[position] != '\n') position++;
        }
        else {
            while (!is_line_end(data[position])) {
                if (data[position] == '"') {
                    preprocessor_token_t * token = preprocessor_token_buffer_push(&preprocessor->tbuf);

                    token->file_name = preprocessor->file_name;
                    token->line = line;

                    size_t string_start = position;
                    size_t string_len = 0;

                    position++;

                    while (true) {
                        if (is_line_end(data[position])) {
                            size_t line_length;
                            for (line_length = 0; data[line_start + line_length] != '\n'; line_length++);
                            fatal_error(
                                "%.*s\n%*c\nUnclosed string at line %zu, character %zu\n",
                                line_length, &data[line_start],
                                string_start - line_start + 1, '^',
                                line + 1,
                                string_start - line_start + 1
                            );
                        }
                        else if (data[position] == '\\') {
                            position++;
                            string_len++;

                            if (data[position] == '"') position++;
                        }
                        else if (data[position] == '"') {
                            string_len += 2;
                            position++;
                            break;
                        }
                        else {
                            position++;
                            string_len++;
                        }
                    }

                    preprocessor_token_string_init(token, &data[string_start], string_len);

                    log_printf("Preprocessor got string with length of %zu on line %zu\n", token->string.size, line + 1);
                }
                else if (is_symbol_start_char(data[position])) {
                    preprocessor_token_t * token = preprocessor_token_buffer_push(&preprocessor->tbuf);

                    token->file_name = preprocessor->file_name;
                    token->line = line;

                    preprocessor_token_symbol_t * symbol = &token->symbol;

                    const char * symbol_start = &data[position];
                    size_t symbol_size = 0;
                    while (is_symbol_char(data[position + symbol_size])) symbol_size++;
                    position += symbol_size;

                    preprocessor_token_symbol_init(token, symbol_start, symbol_size);

                    log_printf("Preprocessor got symbol with length of %zu on line %zu\n", symbol->size, line + 1);

                    size_t temp_position = position;
                    while (is_whitespace(data[temp_position])) temp_position++;

                    if (data[temp_position] == '(') {
                        // TODO: implement function macros
                    }

                    continue;
                }
                else {
                    preprocessor_token_t * token = preprocessor_token_buffer_push(&preprocessor->tbuf);

                    token->file_name = preprocessor->file_name;
                    token->line = line;

                    preprocessor_token_content_t * content = &token->content;

                    const char * content_start = &data[position];
                    size_t content_size = 0;
                    while (
                        !is_symbol_start_char(data[position + content_size]) &&
                        data[position + content_size] != '\n' &&
                        data[position + content_size] != '\0' &&
                        data[position + content_size] != '"'
                    ) content_size++;
                    position += content_size;

                    preprocessor_token_content_init(token, content_start, content_size);

                    log_printf("Preprocessor got non symbol with length %zu on line %zu\n", content->size, line + 1);
                }
            }
        }

        if (data[position] == '\n') position++;

        line++;
    }

    preprocessor_log_tokens(preprocessor);
}

void preprocessor_render(preprocessor_t * preprocessor, line_buffer_t * line_buffer) {
    size_t current_line = preprocessor->tbuf.tokens[0].line;
    file_name_entry_t * file_name = preprocessor->tbuf.tokens[0].file_name;

    size_t line_size = 0;
    size_t line_capacity = 16;
    char * line = pkcc_alloc(line_capacity);

    for (size_t i = 0; i < preprocessor->tbuf.size; i++) {
        preprocessor_token_t * token = &preprocessor->tbuf.tokens[i];

        if (token->line != current_line || token->file_name != file_name) {
            line_buffer_push_line(line_buffer, line, line_size, file_name, current_line);

            current_line = token->line;
            file_name = token->file_name;

            line_size = 0;
        }

        switch (token->type) {
            case PREPROCESSOR_TOKEN_TYPE_DIRECTIVE: {
                preprocessor_token_directive_t * directive = &token->directive;

                switch (directive->type) {
                    default: break; // TODO: implement
                }

//                line_buffer_push_line(line_buffer, "", 0, token->file_name, token->line);
            } break;

            case PREPROCESSOR_TOKEN_TYPE_SYMBOL: {
                preprocessor_token_symbol_t * symbol = &token->symbol;

                const char * value = preprocessor_vmap_get(&preprocessor->vmap, symbol->symbol, symbol->size);

                if (value != NULL) {
                    // TODO: implement
                }
                else {
                    log_printf("Rendering symbol of length %zu with %zu arguments\n", symbol->size, symbol->arg_count);

                    while (line_size + symbol->size > line_capacity) {
                        while (line_size + symbol->size > line_capacity) line_capacity *= 2;

                        line = pkcc_realloc(line, line_capacity);
                    }

                    memcpy(&line[line_size], symbol->symbol, symbol->size);
                    line_size += symbol->size;
                }
            } break;

            case PREPROCESSOR_TOKEN_TYPE_CONTENT: {
                preprocessor_token_content_t * content = &token->content;

                log_printf("Rendering content of length %zu\n", content->size);

                if (line_size + content->size > line_capacity) {
                    while (line_size + content->size > line_capacity) line_capacity *= 2;

                    line = pkcc_realloc(line, line_capacity);
                }

                memcpy(&line[line_size], content->content, content->size);
                line_size += content->size;
            } break;

            case PREPROCESSOR_TOKEN_TYPE_STRING: {
                preprocessor_token_string_t * string = &token->string;

                log_printf("Rendering string of length %zu\n", string->size);

                if (line_size + string->size > line_capacity) {
                    while (line_size + string->size > line_capacity) line_capacity *= 2;

                    line = pkcc_realloc(line, line_capacity);
                }

                memcpy(&line[line_size], string->content, string->size);
                line_size += string->size;
            }
        }
    }

    line_buffer_push_line(line_buffer, line, line_size, file_name, current_line);

    pkcc_free(line);

    log_printf("BEGIN RENDERED FILE\n");
    for (size_t i = 0; i < line_buffer->size; i++) {
        line_buffer_line_t * _line = &line_buffer->lines[i];

        log_printf("\t%s (%zu): '%s'\n", _line->metadata.file_name->absolute_path, _line->metadata.source_line, _line->line);
    }
    log_printf("END RENDERED FILE\n");
}

void preprocessor_log_tokens(preprocessor_t * preprocessor) {
    for (size_t i = 0; i < preprocessor->tbuf.size; i++) {
        log_printf("Token %zu (", i);
        switch (preprocessor->tbuf.tokens[i].type) {
            case PREPROCESSOR_TOKEN_TYPE_DIRECTIVE: {
                log_printf("DIRECTIVE): \"");

                char temp[preprocessor->tbuf.tokens[i].directive.size + 1];
                memcpy(temp, preprocessor->tbuf.tokens[i].directive.content, preprocessor->tbuf.tokens[i].directive.size);
                temp[preprocessor->tbuf.tokens[i].directive.size] = '\0';
                log_print_escaped(temp);
            } break;
            case PREPROCESSOR_TOKEN_TYPE_SYMBOL: {
                log_printf("SYMBOL): \"");

                char temp[preprocessor->tbuf.tokens[i].symbol.size + 1];
                memcpy(temp, preprocessor->tbuf.tokens[i].symbol.symbol, preprocessor->tbuf.tokens[i].symbol.size);
                temp[preprocessor->tbuf.tokens[i].symbol.size] = '\0';
                log_print_escaped(temp);
            } break;
            case PREPROCESSOR_TOKEN_TYPE_CONTENT: {
                log_printf("CONTENT): \"");

                char temp[preprocessor->tbuf.tokens[i].content.size + 1];
                memcpy(temp, preprocessor->tbuf.tokens[i].content.content, preprocessor->tbuf.tokens[i].content.size);
                temp[preprocessor->tbuf.tokens[i].content.size] = '\0';
                log_print_escaped(temp);
            } break;
            case PREPROCESSOR_TOKEN_TYPE_STRING: {
                log_printf("STRING): \"");

                char temp[preprocessor->tbuf.tokens[i].string.size + 1];
                memcpy(temp, preprocessor->tbuf.tokens[i].string.content, preprocessor->tbuf.tokens[i].string.size);
                temp[preprocessor->tbuf.tokens[i].string.size] = '\0';
                log_print_escaped(temp);
            }
        }
        log_printf("\"\n");
    }
}