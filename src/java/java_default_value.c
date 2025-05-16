#include <parser/type_checker/type_registry.h>

#include <java/java_default_value.h>
#include <java/java_primitive_stringify.h>
#include <java/cast.h>

void java_default_value(java_code_generator_t * jcg, type_checker_type_t * type) {
    if (type->is_base) {
        fprintf(
            jcg->out_file,
            "\t\tldc_w 0\n"
        );
        java_cast(jcg, type, sint_type);
    }
    else {
        switch (type->derived_type.type) {
            case DTT_QUALIFIED: {
                java_default_value(jcg, type->derived_type.qualified.subtype);
            } break;

            case DTT_ARRAY: {
                fprintf(
                    jcg->out_file,
                    "\t\tnewarray %s\n",
                    java_primitive_stringify(type->derived_type.array.subtype)
                );
            } break;

            default: fatal_error("Can't generate default value for non-base type\n");
        }
    }
}