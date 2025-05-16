#include <parser/type_checker/type.h>

#include <debug/error_handler.h>

#include <alloc.h>

void type_checker_type_free(type_checker_type_t * type) {
    if (!type->is_base) {
        switch (type->derived_type.type) {
            case DTT_STRUCT: {
                if (type->derived_type.structure.name != NULL) pkcc_free(type->derived_type.structure.name);

                for (size_t i = 0; i < type->derived_type.structure.subtype_count; i++) {
                    pkcc_free(type->derived_type.structure.subtype_names[i]);
                }
                pkcc_free(type->derived_type.structure.subtype_names);
                pkcc_free(type->derived_type.structure.subtypes);
            } break;

            case DTT_QUALIFIED: {
                pkcc_free(type->derived_type.qualified.qualifiers);
            } break;

            case DTT_FUNCTION: {
                pkcc_free(type->derived_type.function.args);
            } break;

            default: break;
        }
    }
}