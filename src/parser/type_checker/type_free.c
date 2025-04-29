#include <parser/type_checker/type.h>

#include <debug/error_handler.h>

#include <alloc.h>

void type_checker_type_free(type_checker_type_t * type) {
    if (!type->is_base) {
        switch (type->derived_type.type) {
            case DTT_STRUCT: {
                if (type->derived_type.structure.name != NULL) pkcc_free(type->derived_type.structure.name);

                pkcc_free(type->derived_type.structure.subtypes);
            } break;

            case DTT_QUALIFIED: {

            } break;

            default: log_printf("ERROR\n");
        }
    }
}