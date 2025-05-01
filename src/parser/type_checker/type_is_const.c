#include <parser/type_checker/type.h>

bool type_checker_type_is_const(type_checker_type_t * type) {
    if (!type->is_base) {
        if (type->derived_type.type == DTT_QUALIFIED) {
            for (size_t i = 0; i < type->derived_type.qualified.qualifier_count; i++) {
                if (type->derived_type.qualified.qualifiers[i] == DTQ_CONST) return true;
            }
        }
        else if (type->derived_type.type == DTT_ARRAY) {
            return type_checker_type_is_const(type->derived_type.array.subtype);
        }

        return false;
    }
    else return false;
}