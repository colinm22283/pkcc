#pragma once

#include <stddef.h>
#include <stdbool.h>

#include <parser/type_checker/type.h>
#include <parser/type_checker/scope.h>

typedef struct {
    type_checker_type_t * type;
    type_checker_scope_t * scope;
    const char * name;
    bool defined;
} type_checker_variable_t;

typedef struct {
    size_t variable_count, variable_capacity;
    type_checker_variable_t ** variables;
} type_checker_variable_registry_t;

void type_checker_variable_registry_init(type_checker_variable_registry_t * vr);
void type_checker_variable_registry_free(type_checker_variable_registry_t * vr);

type_checker_variable_t * type_checker_variable_registry_add(type_checker_variable_registry_t * vr);
type_checker_variable_t * type_checker_variable_registry_lookup(type_checker_variable_registry_t * vr, const char * name, type_checker_scope_t * scope);
type_checker_variable_t * type_checker_variable_registry_lookup_clashes(type_checker_variable_registry_t * vr, const char * name, type_checker_scope_t * scope);
