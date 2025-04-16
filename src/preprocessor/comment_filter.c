#include <alloc.h>
#include <preprocessor/comment_filter.h>

#define PREPROCESSOR_COMMENT_FILTER_INITIAL_CAPACITY (1024)

void preprocessor_comment_filter_init(preprocessor_comment_filter_t * cf) {
    cf->data_capacity = PREPROCESSOR_COMMENT_FILTER_INITIAL_CAPACITY;
    cf->data_size = 0;
    cf->data = pkcc_alloc(cf->data_capacity);
}

void preprocessor_comment_filter_free(preprocessor_comment_filter_t * cf) {
    pkcc_free(cf->data);
}

static inline void push_char(preprocessor_comment_filter_t * cf, char c) {
    cf->data[cf->data_size++] = c;

    if (cf->data_size == cf->data_capacity) {
        cf->data_capacity *= 2;

        cf->data = pkcc_realloc(cf->data, cf->data_capacity);
    }
}

void preprocessor_comment_filter_parse(preprocessor_comment_filter_t * cf, const char * input) {
    size_t line = 0;
    size_t line_start = 0;
    size_t position = 0;

    while (true) {
        if (input[position] == '\0') break;
        if (input[position] == '"') {
            push_char(cf, '\"');
            position++;

            while (input[position] != '"' && input[position] != '\0') {
                if (input[position] == '\n') {
                    push_char(cf, '\n');

                    line++;
                    line_start = position + 1;
                }

                push_char(cf, input[position]);
                position++;
            }

            push_char(cf, '\"');

            if (input[position] == '\0') break;
        }
        else if (input[position] == '/' && (input[position + 1] == '*' || input[position + 1] == '/')) {
            size_t comment_start = position;

            if (input[position + 1] == '*') {
                position += 2;

                while (true) {
                    if (input[position] == '\0') {
                        size_t line_length;
                        for (line_length = 0; input[line_start + line_length] != '\n'; line_length++);
                        fatal_error(
                            "%.*s\n%*c\nUnclosed multiline comment at line %zu, character %zu\n",
                            line_length, &input[line_start],
                            comment_start - line_start + 1, '^',
                            line + 1,
                            comment_start - line_start + 1
                        );
                    }
                    else if (input[position] == '\n') push_char(cf, '\n');
                    else if (input[position] == '*') {
                        if (input[position + 1] == '/') {
                            position += 2;
                            break;
                        }
                    }

                    position++;
                }

                push_char(cf, '\n');
            }
            else if (input[position + 1] == '/') {
                position += 2;

                while (input[position] != '\n' && input[position] != '\0') position++;

                if (input[position] == '\0') break;

                push_char(cf, '\n');
                line++;
                line_start = position + 1;
            }
        }
        else if (input[position] == '\n') {
            line++;
            line_start = position + 1;

            push_char(cf, input[position]);
        }
        else push_char(cf, input[position]);

        position++;
    }

    push_char(cf, '\0');
}