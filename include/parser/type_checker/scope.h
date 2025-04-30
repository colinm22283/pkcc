#pragma once

#include <stddef.h>
#include <stdbool.h>

#include <alloc.h>

typedef struct type_checker_scope_s {
    struct type_checker_scope_s * parent;
    size_t child_count, child_capacity;
    struct type_checker_scope_s ** children;
    size_t depth;
} type_checker_scope_t;

static inline void type_checker_scope_init(type_checker_scope_t * scope) {
    scope->child_count = 0;
    scope->child_capacity = 1;
    scope->children = pkcc_alloc(scope->child_capacity * sizeof(type_checker_scope_t *));
}

static inline void type_checker_scope_add(type_checker_scope_t * scope, type_checker_scope_t * new_scope) {
    scope->children[scope->child_count++] = new_scope;

    if (scope->child_count == scope->child_capacity) {
        scope->child_capacity *= 2;

        scope->children = pkcc_realloc(scope->children, scope->child_capacity * sizeof(type_checker_scope_t *));
    }
}

static inline bool type_checker_scope_is_parent(type_checker_scope_t * parent, type_checker_scope_t * child) {
    type_checker_scope_t * scope = child;

    while (scope->parent != scope) {
        if (scope == parent) return true;

        scope = scope->parent;
    }

    if (scope == parent) return true;
    else return false;
}