#include <string.h>

#include <parser/rules.h>
#include <parser/type_checker/type_registry.h>
#include <parser/type_checker/registry_parsers/struct.h>
#include <parser/type_checker/registry_parsers/type.h>

#include <alloc.h>

#define TYPE_CHECKER_REGISTRY_DEFAULT_CAPACITY (4)

void type_checker_registry_init(type_checker_registry_t * tr) {
    tr->size = 0;
    tr->capacity = TYPE_CHECKER_REGISTRY_DEFAULT_CAPACITY;
    tr->entries = pkcc_alloc(TYPE_CHECKER_REGISTRY_DEFAULT_CAPACITY * sizeof(type_checker_registry_entry_t *));
}

void type_checker_registry_free(type_checker_registry_t * tr) {
    for (size_t i = 0; i < tr->size; i++) {
        type_checker_type_free(&tr->entries[i]->type);

        pkcc_free(tr->entries[i]);
    }

    pkcc_free(tr->entries);
}

type_checker_type_t * type_checker_registry_parse(type_checker_registry_t * tr, token_buffer_t * token_buffer, syntax_tree_node_list_node_t * node) {
    // TODO: parse the syntax tree
    if (node->token_type == RT_NONTERMINAL) {
        switch (node->nonterminal.nonterminal) {
            case NT_STRUCT: {
                return type_checker_registry_parse_struct(tr, token_buffer, node);
            } break;

            case NT_TYPE: {
                return type_checker_registry_parse_type(tr, token_buffer, node);
            } break;

            default: break;
        }

        fatal_error("Type checker called on invalid node '%s'\n", rules_nonterminal_name(node->nonterminal.nonterminal));
    }
    else fatal_error("Type checker called on invalid node '%s'\n", token_number_stringify(node->terminal.terminal));
}