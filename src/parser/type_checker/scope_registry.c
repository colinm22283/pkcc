#include <parser/rules.h>
#include <parser/type_checker/scope_registry.h>

#include <alloc.h>

type_checker_scope_t * root_scope;

static inline type_checker_scope_t * add_scope(type_checker_scope_registry_t * registry) {
    type_checker_scope_t * new_scope = new_scope = pkcc_alloc(sizeof(type_checker_scope_t));
    registry->scopes[registry->scope_count++] = new_scope;
    type_checker_scope_init(new_scope);

    if (registry->scope_count == registry->scope_capacity) {
        registry->scope_capacity *= 2;

        registry->scopes = pkcc_realloc(registry->scopes, registry->scope_capacity * sizeof(type_checker_scope_t *));
    }

    return new_scope;
}

void type_checker_scope_registry_init(type_checker_scope_registry_t * registry, syntax_tree_t * syntax_tree) {
    registry->syntax_tree = syntax_tree;

    registry->scope_count = 0;
    registry->scope_capacity = 4;

    registry->scopes = pkcc_alloc(registry->scope_capacity * sizeof(type_checker_scope_t *));
}

void type_checker_scope_registry_free(type_checker_scope_registry_t * registry) {
    for (size_t i = 0; i < registry->scope_count; i++) {
        pkcc_free(registry->scopes[i]->children);
        pkcc_free(registry->scopes[i]);
    }
    pkcc_free(registry->scopes);
}

void type_checker_scope_registry_run_recur(
    type_checker_scope_registry_t * registry,
    type_checker_scope_t * parent_scope,
    syntax_tree_node_t * node
) {
    syntax_tree_node_list_node_t * list_node = node->head->next;

    while (list_node != node->tail) {
        list_node->scope = parent_scope;

        if (list_node->token_type == RT_NONTERMINAL) {
            if (
                list_node->nonterminal.nonterminal == NT_CODE_BLOCK ||
                list_node->nonterminal.nonterminal == NT_DECL_FUNC
            ) {
                type_checker_scope_t * new_scope = add_scope(registry);
                type_checker_scope_add(parent_scope, new_scope);
                new_scope->parent = parent_scope;
                new_scope->depth = parent_scope->depth + 1;

                type_checker_scope_registry_run_recur(registry, new_scope, &list_node->nonterminal.tree);
            }
            else {
                type_checker_scope_registry_run_recur(registry, parent_scope, &list_node->nonterminal.tree);
            }
        }

        list_node = list_node->next;
    }
}

void type_checker_scope_registry_run(type_checker_scope_registry_t * registry) {
    root_scope = add_scope(registry);

    root_scope->parent = root_scope;
    root_scope->depth = 0;

    type_checker_scope_registry_run_recur(registry, root_scope, &registry->syntax_tree->head);
}