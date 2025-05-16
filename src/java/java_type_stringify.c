#include <java/java_type_stringify.h>

#include <alloc.h>

char * java_type_stringify(type_checker_type_t * type) {
    size_t size = 0, capacity = 16;
    char * str = pkcc_alloc(capacity);
    str[0] = '\0';

#define ADD_STRING(string) \
    do { \
        size_t string_size = strlen(string); \
        while (size + string_size >= capacity) { \
            capacity *= 2; \
            str = pkcc_realloc(str, capacity); \
        } \
        strcpy(&str[size], string); \
        size += string_size; \
    } \
    while (0);

    if (type->is_base) {
        const char * type_str;

        switch (type->base_type.type) {
            case BTT_VOID: type_str = "V"; break;
            case BTT_BOOL: type_str = "Z"; break;
            case BTT_CHAR: type_str = "C"; break;
            case BTT_SHORT: type_str = "S"; break;
            case BTT_INT: type_str = "I"; break;
            case BTT_LONG:
            case BTT_LONG_LONG: type_str = "J"; break;
            case BTT_FLOAT: type_str = "F"; break;
            case BTT_DOUBLE: type_str = "D"; break;

            default: type_str = ""; break;
        }

        ADD_STRING(type_str);
    }
    else {
        switch (type->derived_type.type) {
            case DTT_QUALIFIED: {
                return java_type_stringify(type->derived_type.qualified.subtype);
            } break;

            case DTT_ARRAY: {
                ADD_STRING("[");

                char * sub_str = java_type_stringify(type->derived_type.qualified.subtype);
                ADD_STRING(sub_str);
                pkcc_free(sub_str);
            } break;

            case DTT_FUNCTION: {
                ADD_STRING("(");

                for (size_t i = 0; i < type->derived_type.function.arg_count; i++) {
                    char * sub_str = java_type_stringify(type->derived_type.function.args[i]);
                    ADD_STRING(sub_str);
                    pkcc_free(sub_str);
                }

                ADD_STRING(")");

                char * sub_str = java_type_stringify(type->derived_type.function.return_type);
                ADD_STRING(sub_str);
                pkcc_free(sub_str);
            } break;

            default: break;
        }
    }

    return str;
}