#include <debug/error_handler.h>
#include <debug/log.h>
#include <parsing/is_symbol_char.h>
#include <parsing/is_whitespace.h>
#include <preprocessor/variable_map.h>
#include <preprocessor/directives/define.h>

size_t preprocessor_directive_define(preprocessor_vmap_t * vmap, const char * data, size_t line) {
    size_t position = 0;

    if (!is_symbol_start_char(data[position])) {
        size_t line_length;
        for (line_length = 0; data[line_length] != '\n'; line_length++);
        fatal_error(
            "%.*s\n%*c\nEncountered invalid define statement at line %zu, character %zu\n",
            line_length, data,
            2, '^',
            line + 1,
            2
        );
    }

    const char * key = &data[position];
    size_t key_len = 0;
    while (is_symbol_char(data[position + key_len])) key_len++;
    position += key_len;

    while (is_whitespace(data[position])) position++;

    const char * value = &data[position];
    size_t value_len = 0;
    while (data[position + value_len] != '\n') value_len++;
    position += value_len;

    preprocessor_vmap_node_t * vmap_node = preprocessor_vmap_map(vmap, key, key_len, value, value_len);

    log_printf("Mapped preprocessor definition '%s' to '%s'\n", vmap_node->key, vmap_node->value);

    return position;
}