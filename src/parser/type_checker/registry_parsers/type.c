#include <parser/rules.h>
#include <parser/type_checker/error.h>
#include <parser/type_checker/registry_parsers/type.h>

#include <debug/log.h>

#include <alloc.h>

type_checker_type_t * type_checker_registry_parse_type(type_checker_registry_t * tr, token_buffer_t * token_buffer, syntax_tree_node_list_node_t * type_node) {
    if (type_node->token_type != RT_NONTERMINAL) type_checker_error();
    if (type_node->nonterminal.nonterminal != NT_TYPE) type_checker_error();

    syntax_tree_node_list_node_t * inner_type_node;
    if (
        type_node->nonterminal.tree.head->next->token_type == RT_NONTERMINAL &&
        type_node->nonterminal.tree.head->next->nonterminal.nonterminal == NT_TYPE_INNER
    ) inner_type_node = type_node->nonterminal.tree.head->next;
    else inner_type_node = type_node->nonterminal.tree.head->next->next;

    size_t qualifier_count = 0, qualifier_capacity = 1;
    type_checker_derived_type_qualifier_t * qualifiers = pkcc_alloc(qualifier_capacity * sizeof(type_checker_derived_type_qualifier_t));

    if (
        type_node->nonterminal.tree.head->next->token_type == RT_NONTERMINAL &&
        type_node->nonterminal.tree.head->next->nonterminal.nonterminal == NT_QUALIFIER
    ) {
        log_printf("Got pre qualifier\n");

        syntax_tree_node_list_node_t * qualifier_terminal_node = type_node->nonterminal.tree.head->next->nonterminal.tree.head->next;
        if (qualifier_terminal_node->token_type != RT_TERMINAL) type_checker_error();

        type_checker_derived_type_qualifier_t qualifier;
        switch (qualifier_terminal_node->terminal.terminal) {
            case token_number_keyword(SCANNER_KEYWORD_TYPE_CONST): {
                qualifier = DTQ_CONST;
            } break;

            default: {
                fatal_error("Got unknown qualifier keyword '%s'\n", token_number_stringify(qualifier_terminal_node->terminal.terminal));
            } break;
        }

        bool found = false;
        for (size_t i = 0; i < qualifier_count; i++) {
            if (qualifiers[i] == qualifier) {
                found = true;
                break;
            }
        }

        if (!found) {
            qualifiers[qualifier_count++] = qualifier;

            if (qualifier_count == qualifier_capacity) {
                qualifier_capacity *= 2;

                qualifiers = pkcc_realloc(qualifiers, qualifier_capacity * sizeof(type_checker_derived_type_qualifier_t));
            }
        }
    }

    if (
        inner_type_node->next != type_node->nonterminal.tree.tail &&
        inner_type_node->next->token_type == RT_NONTERMINAL &&
        inner_type_node->next->nonterminal.nonterminal == NT_QUALIFIER
    ) {
        log_printf("Got post qualifier\n");

        syntax_tree_node_list_node_t * qualifier_terminal_node = type_node->nonterminal.tree.head->next->nonterminal.tree.head->next;
        if (qualifier_terminal_node->token_type != RT_TERMINAL) type_checker_error();

        type_checker_derived_type_qualifier_t qualifier;
        switch (qualifier_terminal_node->terminal.terminal) {
            case token_number_keyword(SCANNER_KEYWORD_TYPE_CONST): {
                qualifier = DTQ_CONST;
            } break;

            default: {
                fatal_error("Got unknown qualifier keyword '%s'\n", token_number_stringify(qualifier_terminal_node->terminal.terminal));
            } break;
        }

        bool found = false;
        for (size_t i = 0; i < qualifier_count; i++) {
            if (qualifiers[i] == qualifier) {
                found = true;
                break;
            }
        }

        if (!found) {
            qualifiers[qualifier_count++] = qualifier;

            if (qualifier_count == qualifier_capacity) {
                qualifier_capacity *= 2;

                qualifiers = pkcc_realloc(qualifiers, qualifier_capacity * sizeof(type_checker_derived_type_qualifier_t));
            }
        }
    }

    type_checker_registry_entry_t * entry = pkcc_alloc(sizeof(type_checker_registry_entry_t));
    tr->entries[tr->size++] = entry;
    if (tr->size == tr->capacity) {
        tr->capacity *= 2;

        tr->entries = pkcc_realloc(tr->entries, tr->capacity * sizeof(type_checker_registry_entry_t *));
    }

    if (qualifier_count > 0) {
        entry->type.is_base = false;

        fatal_error("NO QUALIFERS!\n");

        // TODO: aaaaaaaaa
    }
    else {
        pkcc_free(qualifiers);

        entry->type.is_base = true;

        syntax_tree_node_list_node_t * inner_type_sub_node = inner_type_node->nonterminal.tree.head->next;

        if (inner_type_sub_node->token_type == RT_TERMINAL) {
            switch (inner_type_sub_node->terminal.terminal) {
                case token_number_keyword(SCANNER_KEYWORD_TYPE_VOID): entry->type.base_type.type = BTT_VOID; break;
                case token_number_keyword(SCANNER_KEYWORD_TYPE_BOOL): entry->type.base_type.type = BTT_BOOL; break;
                case token_number_keyword(SCANNER_KEYWORD_TYPE_CHAR): entry->type.base_type.type = BTT_CHAR; break;
                case token_number_keyword(SCANNER_KEYWORD_TYPE_SHORT): entry->type.base_type.type = BTT_SHORT; break;
                case token_number_keyword(SCANNER_KEYWORD_TYPE_INT): entry->type.base_type.type = BTT_INT; break;
                case token_number_keyword(SCANNER_KEYWORD_TYPE_LONG): entry->type.base_type.type = BTT_LONG; break;
                case token_number_keyword(SCANNER_KEYWORD_TYPE_FLOAT): entry->type.base_type.type = BTT_FLOAT; break;
                case token_number_keyword(SCANNER_KEYWORD_TYPE_DOUBLE): entry->type.base_type.type = BTT_DOUBLE; break;
                // TODO: long long

                default: fatal_error("Unknown type received\n");
            }

            entry->type.base_type.sign = BTS_SIGNED; // TODO: support unsigned
        }
        else {
            return type_checker_registry_parse(tr, token_buffer, inner_type_sub_node);
        }
    }

    return &entry->type;
}
