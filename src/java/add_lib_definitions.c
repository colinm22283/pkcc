#include <java/add_lib_definitions.h>

#include <parser/type_checker/scope_registry.h>


type_checker_type_t * java_getchar_type = NULL;
type_checker_type_t * java_putchar_type = NULL;
type_checker_type_t * java_getint_type = NULL;
type_checker_type_t * java_putint_type = NULL;
type_checker_type_t * java_getfloat_type = NULL;
type_checker_type_t * java_putfloat_type = NULL;
type_checker_type_t * java_putstring_type = NULL;

static inline type_checker_registry_entry_t * add_entry(type_checker_registry_t * tr) {
    type_checker_registry_entry_t * entry = pkcc_alloc(sizeof(type_checker_registry_entry_t));
    tr->entries[tr->size++] = entry;
    if (tr->size == tr->capacity) {
        tr->capacity *= 2;

        tr->entries = pkcc_realloc(tr->entries, tr->capacity * sizeof(type_checker_registry_entry_t *));
    }
    return entry;
}

void java_add_lib_definitions(type_checker_registry_t * tr, type_checker_variable_registry_t * vr) {
    {
        type_checker_variable_t * variable = type_checker_variable_registry_add(vr);

        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->scope = root_scope;
        entry->type.is_base = false;
        entry->type.derived_type.type = DTT_FUNCTION;
        entry->type.derived_type.function.return_type = sint_type;
        entry->type.derived_type.function.arg_count = 0;
        entry->type.derived_type.function.args = pkcc_alloc(1);

        java_getchar_type = &entry->type;

        variable->name = "getchar";
        variable->scope = root_scope;
        variable->defined = false;
        variable->type = &entry->type;
    }

    {
        type_checker_variable_t * variable = type_checker_variable_registry_add(vr);

        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->scope = root_scope;
        entry->type.is_base = false;
        entry->type.derived_type.type = DTT_FUNCTION;
        entry->type.derived_type.function.return_type = sint_type;
        entry->type.derived_type.function.arg_count = 1;
        entry->type.derived_type.function.args = pkcc_alloc(entry->type.derived_type.function.arg_count * sizeof(type_checker_type_t *));
        entry->type.derived_type.function.args[0] = sint_type;

        java_putchar_type = &entry->type;

        variable->name = "putchar";
        variable->scope = root_scope;
        variable->defined = false;
        variable->type = &entry->type;
    }

    {
        type_checker_variable_t * variable = type_checker_variable_registry_add(vr);

        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->scope = root_scope;
        entry->type.is_base = false;
        entry->type.derived_type.type = DTT_FUNCTION;
        entry->type.derived_type.function.return_type = sint_type;
        entry->type.derived_type.function.arg_count = 0;
        entry->type.derived_type.function.args = pkcc_alloc(1);

        java_getint_type = &entry->type;

        variable->name = "getint";
        variable->scope = root_scope;
        variable->defined = false;
        variable->type = &entry->type;
    }

    {
        type_checker_variable_t * variable = type_checker_variable_registry_add(vr);

        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->scope = root_scope;
        entry->type.is_base = false;
        entry->type.derived_type.type = DTT_FUNCTION;
        entry->type.derived_type.function.return_type = void_type;
        entry->type.derived_type.function.arg_count = 1;
        entry->type.derived_type.function.args = pkcc_alloc(entry->type.derived_type.function.arg_count * sizeof(type_checker_type_t *));
        entry->type.derived_type.function.args[0] = sint_type;

        java_putint_type = &entry->type;

        variable->name = "putint";
        variable->scope = root_scope;
        variable->defined = false;
        variable->type = &entry->type;
    }

    {
        type_checker_variable_t * variable = type_checker_variable_registry_add(vr);

        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->scope = root_scope;
        entry->type.is_base = false;
        entry->type.derived_type.type = DTT_FUNCTION;
        entry->type.derived_type.function.return_type = float_type;
        entry->type.derived_type.function.arg_count = 0;
        entry->type.derived_type.function.args = pkcc_alloc(1);

        java_getfloat_type = &entry->type;

        variable->name = "getfloat";
        variable->scope = root_scope;
        variable->defined = false;
        variable->type = &entry->type;
    }

    {
        type_checker_variable_t * variable = type_checker_variable_registry_add(vr);

        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->scope = root_scope;
        entry->type.is_base = false;
        entry->type.derived_type.type = DTT_FUNCTION;
        entry->type.derived_type.function.return_type = void_type;
        entry->type.derived_type.function.arg_count = 1;
        entry->type.derived_type.function.args = pkcc_alloc(entry->type.derived_type.function.arg_count * sizeof(type_checker_type_t *));
        entry->type.derived_type.function.args[0] = float_type;

        java_putfloat_type = &entry->type;

        variable->name = "putfloat";
        variable->scope = root_scope;
        variable->defined = false;
        variable->type = &entry->type;
    }

    {
        type_checker_variable_t * variable = type_checker_variable_registry_add(vr);

        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->scope = root_scope;
        entry->type.is_base = false;
        entry->type.derived_type.type = DTT_FUNCTION;
        entry->type.derived_type.function.return_type = void_type;
        entry->type.derived_type.function.arg_count = 1;
        entry->type.derived_type.function.args = pkcc_alloc(entry->type.derived_type.function.arg_count * sizeof(type_checker_type_t *));
        entry->type.derived_type.function.args[0] = schararr_type;

        java_putstring_type = &entry->type;

        variable->name = "putstring";
        variable->scope = root_scope;
        variable->defined = false;
        variable->type = &entry->type;
    }
}
