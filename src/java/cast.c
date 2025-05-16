#include <java/cast.h>
#include <java/java_prefix.h>

bool java_cast(java_code_generator_t * jcg, type_checker_type_t * target_type, type_checker_type_t * source_type) {
    if (!target_type->is_base) {
        switch (target_type->derived_type.type) {
            case DTT_QUALIFIED:
                return java_cast(jcg, target_type->derived_type.qualified.subtype, source_type);

            default:
                return false;
        }
    }

    if (!source_type->is_base) {
        switch (source_type->derived_type.type) {
            case DTT_QUALIFIED:
                return java_cast(jcg, target_type, source_type->derived_type.qualified.subtype);

            default:
                return false;
        }
    }

    if (
        (
            (
                target_type->base_type.type == BTT_FLOAT ||
                target_type->base_type.type == BTT_DOUBLE
            ) && (
                source_type->base_type.type == BTT_BOOL ||
                source_type->base_type.type == BTT_CHAR ||
                source_type->base_type.type == BTT_SHORT ||
                source_type->base_type.type == BTT_INT ||
                source_type->base_type.type == BTT_LONG ||
                source_type->base_type.type == BTT_LONG_LONG
            )
        ) || (
            (
                source_type->base_type.type == BTT_FLOAT ||
                source_type->base_type.type == BTT_DOUBLE
            ) && (
                 target_type->base_type.type == BTT_BOOL ||
                 target_type->base_type.type == BTT_CHAR ||
                 target_type->base_type.type == BTT_SHORT ||
                 target_type->base_type.type == BTT_INT ||
                 target_type->base_type.type == BTT_LONG ||
                 target_type->base_type.type == BTT_LONG_LONG
            )
        ) ||
        (
            source_type->base_type.type == BTT_FLOAT &&
            target_type->base_type.type == BTT_DOUBLE
        ) ||
        (
            source_type->base_type.type == BTT_DOUBLE &&
            target_type->base_type.type == BTT_FLOAT
        )
    ) {
        fprintf(jcg->out_file, "\t\t%c2%c\n", java_prefix(source_type), java_prefix(target_type));
    }

    return true;
}