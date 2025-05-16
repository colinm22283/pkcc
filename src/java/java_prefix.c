#include <java/java_prefix.h>

char java_prefix(type_checker_type_t * type) {
    if (type->is_base) {
        switch (type->base_type.type) {
            case BTT_VOID: return 'X';

            case BTT_BOOL: return 'i';

            case BTT_CHAR: return 'c';

            case BTT_SHORT: return 's';

            case BTT_INT: return 'i';

            case BTT_LONG:
            case BTT_LONG_LONG: return 'l';

            case BTT_FLOAT: return 'f';

            case BTT_DOUBLE: return 'd';

            default: return 'X';
        }
    }
    else {
        switch (type->derived_type.type) {
            case DTT_QUALIFIED: return java_prefix(type->derived_type.qualified.subtype);
            case DTT_ARRAY: return 'a';

            default: return 'X';
        }
    }
}

char java_prefix_reduced(type_checker_type_t * type) {
    if (type->is_base) {
        switch (type->base_type.type) {
            case BTT_VOID: return 'X';

            case BTT_BOOL: return 'b';

            case BTT_CHAR: return 'i';

            case BTT_SHORT: return 's';

            case BTT_INT: return 'i';

            case BTT_LONG:
            case BTT_LONG_LONG: return 'l';

            case BTT_FLOAT: return 'f';

            case BTT_DOUBLE: return 'd';

            default: return 'X';
        }
    }
    else {
        switch (type->derived_type.type) {
            case DTT_QUALIFIED: return java_prefix(type->derived_type.qualified.subtype);
            case DTT_ARRAY: return 'a';

            default: return 'X';
        }
    }
}