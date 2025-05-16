#include <parser/type_checker/type.h>

#include <debug/log.h>

type_checker_type_convertable_result_t type_checker_type_convertable(type_checker_type_t * to, type_checker_type_t * from) {
    if (to->is_base && from->is_base) {
        if (type_checker_type_sizes[to->base_type.type] < type_checker_type_sizes[from->base_type.type]) {
            return TCR_NARROWING;
        }

        return TCR_CONVERTABLE;
    }
    else {
        if (!to->is_base && to->derived_type.type == DTT_QUALIFIED) {
            if (to->derived_type.qualified.qualifier_count == 1) {
                if (to->derived_type.qualified.qualifiers[0] == DTQ_CONST) {
                    return type_checker_type_convertable(to->derived_type.qualified.subtype, from);
                }
            }
        }

        if (!from->is_base && from->derived_type.type == DTT_QUALIFIED) {
            if (from->derived_type.qualified.qualifier_count == 1) {
                if (from->derived_type.qualified.qualifiers[0] == DTQ_CONST) {
                    return type_checker_type_convertable(to, from->derived_type.qualified.subtype);
                }
            }
        }

        if (
            !from->is_base &&
            from->derived_type.type == DTT_ARRAY &&
            !to->is_base &&
            to->derived_type.type == DTT_ARRAY
        ) {
            return type_checker_type_convertable(to->derived_type.array.subtype, from->derived_type.array.subtype);
        }

        return type_checker_type_equal(to, from) ? TCR_CONVERTABLE : TCR_NOT_CONVERTABLE;
    }
}