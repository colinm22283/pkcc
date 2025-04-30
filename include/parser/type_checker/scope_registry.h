#pragma once

#include <parser/syntax_tree.h>
#include <parser/type_checker/scope.h>

typedef struct {
    syntax_tree_t * syntax_tree;

    size_t scope_count, scope_capacity;
    type_checker_scope_t ** scopes;
} type_checker_scope_registry_t;

extern type_checker_scope_t * root_scope;

void type_checker_scope_registry_init(type_checker_scope_registry_t * registry, syntax_tree_t * syntax_tree);
void type_checker_scope_registry_free(type_checker_scope_registry_t * registry);

void type_checker_scope_registry_run(type_checker_scope_registry_t * registry);