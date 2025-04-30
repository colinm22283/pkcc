#include <parser/type_checker/variable_registry.h>

#include <alloc.h>

void type_checker_variable_registry_init(type_checker_variable_registry_t * vr) {
    vr->variable_count = 0;
    vr->variable_capacity = 4;
    vr->variables = pkcc_alloc(vr->variable_capacity * sizeof(type_checker_variable_t *));
}

void type_checker_variable_registry_free(type_checker_variable_registry_t * vr) {
    for (size_t i = 0; i < vr->variable_count; i++) {
        pkcc_free(vr->variables[i]);
    }

    pkcc_free(vr->variables);
}

type_checker_variable_t * type_checker_variable_registry_add(type_checker_variable_registry_t * vr) {
    vr->variables[vr->variable_count] = pkcc_alloc(sizeof(type_checker_variable_t));
    type_checker_variable_t * new_variable = vr->variables[vr->variable_count++];

    if (vr->variable_count == vr->variable_capacity) {
        vr->variable_capacity *= 2;

        vr->variables = pkcc_realloc(vr->variables, vr->variable_capacity * sizeof(type_checker_variable_t *));
    }

    return new_variable;
}

type_checker_variable_t * type_checker_variable_registry_lookup(type_checker_variable_registry_t * vr, const char * name, type_checker_scope_t * scope) {
    for (size_t i = 0; i < vr->variable_count; i++) {
        if (
            vr->variables[i]->scope->depth != 0 &&
            type_checker_scope_is_parent(vr->variables[i]->scope, scope) &&
            strcmp(name, vr->variables[i]->name) == 0
        ) return vr->variables[i];
    }

    for (size_t i = 0; i < vr->variable_count; i++) {
        if (
            vr->variables[i]->scope->depth == 0 &&
            type_checker_scope_is_parent(vr->variables[i]->scope, scope) &&
            strcmp(name, vr->variables[i]->name) == 0
        ) return vr->variables[i];
    }

    return NULL;
}

type_checker_variable_t * type_checker_variable_registry_lookup_clashes(type_checker_variable_registry_t * vr, const char * name, type_checker_scope_t * scope) {
    for (size_t i = 0; i < vr->variable_count; i++) {
        if (vr->variables[i]->scope->depth == 0) {
            if (scope->depth == 0) {
                if (strcmp(name, vr->variables[i]->name) == 0) return vr->variables[i];
            }
        }
        else {
            if (
                type_checker_scope_is_parent(vr->variables[i]->scope, scope) &&
                strcmp(name, vr->variables[i]->name) == 0
                ) return vr->variables[i];
        }
    }

    return NULL;
}