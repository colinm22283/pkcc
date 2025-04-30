#include <parser/type_checker/type.h>

#include <debug/line_warning.h>

type_checker_type_convertable_result_t type_checker_type_convertable(type_checker_type_t * to, type_checker_type_t * from) {
    if (to->is_base && from->is_base) {
        if (type_checker_type_sizes[to->base_type.type] < type_checker_type_sizes[from->base_type.type]) {
            return TCR_NARROWING;
        }

        return TCR_CONVERTABLE;
    }
    else {
        return type_checker_type_equal(to, from) ? TCR_CONVERTABLE : TCR_NOT_CONVERTABLE;
    }
}