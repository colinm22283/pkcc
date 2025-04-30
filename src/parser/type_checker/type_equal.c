#include <string.h>

#include <parser/type_checker/type.h>

bool type_checker_type_equal(type_checker_type_t * a, type_checker_type_t * b) {
    if (a->is_base && b->is_base) {
        if (a->base_type.type != b->base_type.type) return false;
        if (a->base_type.sign != b->base_type.sign) return false;
    }
    else if (!a->is_base && !b->is_base) {
        if (a->derived_type.type != b->derived_type.type) return false;

        switch (a->derived_type.type) {
            case DTT_ARRAY: {
                if (a->derived_type.array.has_size != b->derived_type.array.has_size) return false;
                if (a->derived_type.array.has_size && a->derived_type.array.size != b->derived_type.array.size) return false;
                if (!type_checker_type_equal(a->derived_type.array.subtype, b->derived_type.array.subtype)) return false;
            } break;

            case DTT_QUALIFIED: {
                if (a->derived_type.qualified.qualifier_count != b->derived_type.qualified.qualifier_count) return false;

                // TODO: do this

                if (!type_checker_type_equal(a->derived_type.qualified.subtype, b->derived_type.qualified.subtype)) return false;
            } break;

            case DTT_POINTER: {
                return false; // TODO: implement
            } break;

            case DTT_STRUCT: {
                if (a->derived_type.structure.subtype_count != b->derived_type.structure.subtype_count) return false;
                if (strcmp(a->derived_type.structure.name, b->derived_type.structure.name) != 0) return false;
                for (size_t i = 0; i < a->derived_type.structure.subtype_count; i++) {
                    if (!type_checker_type_equal(a->derived_type.structure.subtypes[i], b->derived_type.structure.subtypes[i])) {
                        return false;
                    }
                }
            } break;

            case DTT_FUNCTION: {
                if (a->derived_type.function.arg_count != b->derived_type.function.arg_count) return false;
                if (!type_checker_type_equal(a->derived_type.function.return_type, b->derived_type.function.return_type)) return false;
                for (size_t i = 0; i < a->derived_type.function.arg_count; i++) {
                    if (!type_checker_type_equal(a->derived_type.function.args[i], b->derived_type.function.args[i])) {
                        return false;
                    }
                }
            }
        }
    }
    else return false;

    return true;
}