#include <java/java_primitive_stringify.h>

const char * java_primitive_stringify(type_checker_type_t * type) {
    if (!type->is_base) return "NON_BASE";

    switch (type->base_type.type) {
        case BTT_BOOL: return "boolean";
        case BTT_CHAR: return "char";
        case BTT_SHORT: return "short";
        case BTT_INT: return "int";
        case BTT_LONG:
        case BTT_LONG_LONG: return "long";
        case BTT_FLOAT: return "float";
        case BTT_DOUBLE: return "double";

        default: return "INVALID";
    }
}