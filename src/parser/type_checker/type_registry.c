#include <string.h>

#include <parser/rules.h>
#include <parser/type_checker/type_registry.h>
#include <parser/type_checker/registry_parsers/struct.h>
#include <parser/type_checker/registry_parsers/type.h>
#include <parser/type_checker/registry_parsers/expression.h>
#include <parser/type_checker/registry_parsers/terminal.h>

#include <alloc.h>

#define TYPE_CHECKER_REGISTRY_DEFAULT_CAPACITY (4)

type_checker_type_t * void_type;
type_checker_type_t * bool_type;
type_checker_type_t * uchar_type;
type_checker_type_t * ushort_type;
type_checker_type_t * uint_type;
type_checker_type_t * ulong_type;
type_checker_type_t * ulonglong_type;
type_checker_type_t * schar_type;
type_checker_type_t * sshort_type;
type_checker_type_t * sint_type;
type_checker_type_t * slong_type;
type_checker_type_t * slonglong_type;
type_checker_type_t * float_type;
type_checker_type_t * double_type;

static inline type_checker_registry_entry_t * add_entry(type_checker_registry_t * tr) {
    type_checker_registry_entry_t * entry = pkcc_alloc(sizeof(type_checker_registry_entry_t));
    tr->entries[tr->size++] = entry;
    if (tr->size == tr->capacity) {
        tr->capacity *= 2;

        tr->entries = pkcc_realloc(tr->entries, tr->capacity * sizeof(type_checker_registry_entry_t *));
    }
    return entry;
}

void type_checker_registry_init(type_checker_registry_t * tr) {
    tr->size = 0;
    tr->capacity = TYPE_CHECKER_REGISTRY_DEFAULT_CAPACITY;
    tr->entries = pkcc_alloc(TYPE_CHECKER_REGISTRY_DEFAULT_CAPACITY * sizeof(type_checker_registry_entry_t *));

    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_VOID;
        entry->type.base_type.sign = BTS_UNSIGNED;
        void_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_BOOL;
        entry->type.base_type.sign = BTS_UNSIGNED;
        bool_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_CHAR;
        entry->type.base_type.sign = BTS_UNSIGNED;
        uchar_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_SHORT;
        entry->type.base_type.sign = BTS_UNSIGNED;
        ushort_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_INT;
        entry->type.base_type.sign = BTS_UNSIGNED;
        uint_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_LONG;
        entry->type.base_type.sign = BTS_UNSIGNED;
        ulong_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_LONG_LONG;
        entry->type.base_type.sign = BTS_UNSIGNED;
        ulonglong_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_CHAR;
        entry->type.base_type.sign = BTS_SIGNED;
        schar_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_SHORT;
        entry->type.base_type.sign = BTS_SIGNED;
        sshort_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_INT;
        entry->type.base_type.sign = BTS_SIGNED;
        sint_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_LONG;
        entry->type.base_type.sign = BTS_SIGNED;
        slong_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_LONG_LONG;
        entry->type.base_type.sign = BTS_SIGNED;
        slonglong_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_FLOAT;
        entry->type.base_type.sign = BTS_SIGNED;
        float_type = &entry->type;
    }
    {
        type_checker_registry_entry_t * entry = add_entry(tr);
        entry->type.is_base = true;
        entry->type.base_type.type = BTT_DOUBLE;
        entry->type.base_type.sign = BTS_SIGNED;
        double_type = &entry->type;
    }
}

void type_checker_registry_free(type_checker_registry_t * tr) {
    for (size_t i = 0; i < tr->size; i++) {
        type_checker_type_free(&tr->entries[i]->type);

        pkcc_free(tr->entries[i]);
    }

    pkcc_free(tr->entries);
}

type_checker_type_t * type_checker_registry_parse(type_checker_registry_t * tr, line_buffer_t * line_buffer, token_buffer_t * token_buffer, syntax_tree_node_list_node_t * node) {
    // TODO: parse the syntax tree
    if (node->token_type == RT_NONTERMINAL) {
        switch (node->nonterminal.nonterminal) {
            case NT_STRUCT: {
                return type_checker_registry_parse_struct(tr, line_buffer, token_buffer, node);
            } break;

            case NT_TYPE: {
                return type_checker_registry_parse_type(tr, line_buffer, token_buffer, node);
            } break;

            default: {
                if (nonterminal_is_expression(node->nonterminal.nonterminal)) {
                    return type_checker_registry_parse_expression(tr, line_buffer, token_buffer, node);
                }
            } break;
        }
    }
    else {
        return type_checker_registry_parse_terminal(tr, token_buffer, node);
    }

    return NULL;
}