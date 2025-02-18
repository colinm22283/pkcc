#include <stdbool.h>

#include <alloc.h>

#include <preprocessor/preprocessor.h>
#include <preprocessor/directives/define.h>
#include <preprocessor/directives/undef.h>
#include <parsing/is_whitespace.h>
#include <parsing/is_symbol_char.h>
#include <parsing/is_line_end.h>

void preprocessor_init(preprocessor_t * preprocessor) {
    preprocessor->output_size = 0;
    preprocessor->output_capacity = PREPROCESSOR_DEFAULT_CAPACITY;
    preprocessor->output = pkcc_alloc(preprocessor->output_capacity);

    preprocessor_token_buffer_init(&preprocessor->tbuf);
    preprocessor_vmap_init(&preprocessor->vmap);

    preprocessor_comment_filter_init(&preprocessor->cfilter);
}

void preprocessor_free(preprocessor_t * preprocessor) {
    preprocessor_comment_filter_free(&preprocessor->cfilter);

    preprocessor_vmap_free(&preprocessor->vmap);
    preprocessor_token_buffer_free(&preprocessor->tbuf);

    pkcc_free(preprocessor->output);
}

static inline void push_char(preprocessor_t * preprocessor, char c) {
    preprocessor->output[preprocessor->output_size++] = c;

    if (preprocessor->output_size == preprocessor->output_capacity) {
        preprocessor->output_capacity *= 2;

        preprocessor->output = pkcc_realloc(preprocessor->output, preprocessor->output_capacity);
    }
}

void preprocessor_parse(preprocessor_t * preprocessor, const char * input_data) {
    preprocessor_comment_filter_parse(&preprocessor->cfilter, input_data);

    const char * data = preprocessor->cfilter.data;

    size_t position = 0;
    size_t line = 0;
    while (true) {
        size_t line_start = position;

        while (is_whitespace(data[position])) position++;

        if (data[position] == '\0') break;
        else if (data[position] == '#') {
            log_printf("Got preprocessor directive on line %zu\n", line + 1);

            position++;

            if (strncmp(&data[position], "include", 7) == 0) {
                position += 7;

                while (is_whitespace(data[position])) position++;

                char start_char = data[position];
                bool relative_include;

                if (start_char == '<') relative_include = false;
                else if (start_char == '"') relative_include = true;
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
                }
                position++;

                const char * path = &data[position];
                size_t path_size = 0;
                while (data[position + path_size] != start_char) {
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
            while (data[position] != '\n') {
                if (is_symbol_start_char(data[position])) {
                    preprocessor_token_t * token = preprocessor_token_buffer_push(&preprocessor->tbuf);

                    token->type = PREPROCESSOR_TOKEN_TYPE_SYMBOL;
                    preprocessor_token_symbol_t * symbol = &token->symbol;

                    symbol->symbol = &data[position];
                    symbol->size = 0;
                    while (is_symbol_char(data[position + symbol->size])) symbol->size++;
                    position += symbol->size;

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

                    token->type = PREPROCESSOR_TOKEN_TYPE_CONTENT;
                    preprocessor_token_content_t * content = &token->content;

                    content->content = &data[position];
                    content->size = 0;
                    while (!is_symbol_start_char(data[position + content->size]) && data[position + content->size] != '\n') content->size++;
                    position += content->size;
                    if (data[position] == '\n') content->size++;

                    log_printf("Preprocessor got non symbol with length %zu on line %zu\n", content->size, line + 1);
                }
            }
        }

        line++;
        position++;
    }


}

void preprocessor_render(preprocessor_t * preprocessor) {
    for (size_t i = 0; i < preprocessor->tbuf.size; i++) {
        preprocessor_token_t * token = &preprocessor->tbuf.tokens[i];

        switch (token->type) {
            case PREPROCESSOR_TOKEN_TYPE_DIRECTIVE: {
                preprocessor_token_directive_t * directive = &token->directive;

                switch (directive->type) {
                    default: break; // TODO: implement
                }

                push_char(preprocessor, '\n');
            } break;

            case PREPROCESSOR_TOKEN_TYPE_SYMBOL: {
                preprocessor_token_symbol_t * symbol = &token->symbol;

                const char * value = preprocessor_vmap_get(&preprocessor->vmap, symbol->symbol, symbol->size);

                if (value != NULL) {
                    // TODO: implement
                }
                else {
                    log_printf("Rendering symbol of length %zu with %zu arguments\n", symbol->size, symbol->arg_count);

                    for (size_t j = 0; j < symbol->size; j++) {
                        push_char(preprocessor, symbol->symbol[j]);
                    }
                }
            } break;

            case PREPROCESSOR_TOKEN_TYPE_CONTENT: {
                preprocessor_token_content_t * content = &token->content;

                log_printf("Rendering content of length %zu\n", content->size);

                for (size_t j = 0; j < content->size; j++) push_char(preprocessor, content->content[j]);
            } break;
        }
    }

    push_char(preprocessor, '\0');

    log_printf("BEGIN RENDERED FILE\n%s\nEND RENDERED FILE\n", preprocessor->output);
}