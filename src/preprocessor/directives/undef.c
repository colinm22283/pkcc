#include <debug/log.h>
#include <parsing/is_whitespace.h>
#include <parsing/is_symbol_char.h>
#include <preprocessor/directives/undef.h>

size_t preprocessor_directive_undef(preprocessor_vmap_t * vmap, const char * data, size_t line) {
    size_t position = 0;

    while (is_whitespace(data[position])) position++;

    const char * key = &data[position];
    size_t key_len = 0;
    while (is_symbol_char(data[position + key_len])) key_len++;
    position += key_len;

    bool result = preprocessor_vmap_unmap(vmap, key, key_len);

    if (result) log_printf("Undefined preprocessor definition '%.*s'\n", key_len, key);
    else log_printf("Attempted to unmap preprocessor definition '%.*s'\n", key_len, key);

    return position;
}