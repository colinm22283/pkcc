#include <stddef.h>

#include <parser/type_checker/type.h>

#include <alloc.h>

char * type_checker_type_stringify(type_checker_type_t * type) {
    size_t size = 0, capacity = 16;
    char * str = pkcc_alloc(capacity);

    #define PUSH_CHAR(c) do { \
        str[size++] = (c); \
        if (size == capacity) { \
            capacity *= 2; \
            str = pkcc_realloc(str, capacity); \
        } \
    } while (0)

    #define PUSH_STR(s) do { \
        for (size_t _i = 0; (s)[_i] != '\0'; _i++) PUSH_CHAR((s)[_i]); \
    } while (0)

    if (type->is_base) {
        if (type->base_type.sign == BTS_UNSIGNED) PUSH_STR("unsigned ");

        switch (type->base_type.type) {
            case BTT_VOID: PUSH_STR("void"); break;
            case BTT_BOOL: PUSH_STR("bool"); break;
            case BTT_CHAR: PUSH_STR("char"); break;
            case BTT_SHORT: PUSH_STR("short"); break;
            case BTT_INT: PUSH_STR("int"); break;
            case BTT_LONG: PUSH_STR("long"); break;
            case BTT_LONG_LONG: PUSH_STR("long long"); break;
            case BTT_FLOAT: PUSH_STR("float"); break;
            case BTT_DOUBLE: PUSH_STR("double"); break;
        }
    }
    else {
        switch (type->derived_type.type) {
            case DTT_STRUCT: {
                PUSH_STR("struct ");

                if (type->derived_type.structure.name == NULL) PUSH_STR("<anonymous> ");
                else {
                    PUSH_STR(type->derived_type.structure.name);
                    PUSH_CHAR(' ');
                }

                if (type->derived_type.structure.subtypes != NULL) {
                    PUSH_STR("{ ");

                    for (size_t i = 0; i < type->derived_type.structure.subtype_count; i++) {
                        char * type_string = type_checker_type_stringify(type->derived_type.structure.subtypes[i]);
                        PUSH_STR(type_string);
                        pkcc_free(type_string);

                        if (type->derived_type.structure.subtype_names[i] != NULL) {
                            PUSH_CHAR(' ');
                            PUSH_STR(type->derived_type.structure.subtype_names[i]);
                        }

                        if (i != type->derived_type.structure.subtype_count - 1) PUSH_STR(", ");
                    }

                    PUSH_STR(" }");
                }
            } break;

            case DTT_QUALIFIED: {
                char * type_string = type_checker_type_stringify(type->derived_type.qualified.subtype);
                PUSH_STR(type_string);
                pkcc_free(type_string);

                for (size_t i = 0; i < type->derived_type.qualified.qualifier_count; i++) {
                    switch (type->derived_type.qualified.qualifiers[i]) {
                        case DTQ_CONST: PUSH_STR(" const"); break;
                    }
                }
            } break;

            case DTT_ARRAY: {
                char * type_string = type_checker_type_stringify(type->derived_type.array.subtype);
                PUSH_STR(type_string);
                pkcc_free(type_string);

                PUSH_STR("[]");
            } break;

            case DTT_FUNCTION: {
                char * type_string;

                type_string = type_checker_type_stringify(type->derived_type.function.return_type);
                PUSH_STR(type_string);
                pkcc_free(type_string);

                PUSH_STR("(");

                for (size_t i = 0; i < type->derived_type.function.arg_count; i++) {
                    type_string = type_checker_type_stringify(type->derived_type.function.args[i]);
                    PUSH_STR(type_string);
                    pkcc_free(type_string);

                    if (i != type->derived_type.function.arg_count - 1) PUSH_STR(", ");
                }

                PUSH_STR(")");
            } break;

//            default: fatal_error("Unimplemented in stringify %zu\n", type->derived_type.type);
        }
    }

    PUSH_CHAR('\0');

    return str;
}