#include <parser/type_checker/error.h>
#include <parser/type_checker/registry_parsers/decl_var.h>

#include <debug/line_error.h>

type_checker_type_t * type_checker_registry_parse_decl_var(
    __MAYBE_UNUSED type_checker_registry_t * tr,
    __MAYBE_UNUSED type_checker_variable_registry_t * vr,
    __MAYBE_UNUSED line_buffer_t * line_buffer,
    __MAYBE_UNUSED token_buffer_t * token_buffer,
    syntax_tree_node_list_node_t * expression_node
) {
    if (expression_node->token_type != RT_NONTERMINAL) type_checker_error();
    if (expression_node->nonterminal.nonterminal != NT_DECL_VAR) type_checker_error();

    syntax_tree_node_list_node_t * type_node = expression_node->nonterminal.tree.head->next;
    type_checker_type_t * root_type = type_node->type;

    syntax_tree_node_list_node_t * name_list_node;
    if (expression_node->nonterminal.tree.head->next->next != expression_node->nonterminal.tree.tail) {
        name_list_node = expression_node->nonterminal.tree.head->next->next;
    }
    else name_list_node = NULL;

    while (name_list_node != NULL) {
        syntax_tree_node_list_node_t * temp_node = name_list_node->nonterminal.tree.head->next;

        syntax_tree_node_list_node_t * decl_var_decl;
        if (temp_node->token_type != RT_NONTERMINAL) temp_node = temp_node->next;
        decl_var_decl = temp_node;

        syntax_tree_node_list_node_t * inner_temp_node = decl_var_decl->nonterminal.tree.head->next;

        syntax_tree_node_list_node_t * decl_identifier_node = inner_temp_node;
        inner_temp_node = inner_temp_node->next;

        log_printf("Got declaration for '%s'\n", token_buffer->tokens[decl_identifier_node->terminal.position].identifier_data->name);

        syntax_tree_node_list_node_t * array_node = NULL;
        if (
            inner_temp_node != decl_var_decl->nonterminal.tree.tail &&
            inner_temp_node->nonterminal.nonterminal == NT_DECL_VAR_DECL_ARRAY
        ) {
            log_printf("Got array\n");

            array_node = inner_temp_node;
            inner_temp_node = inner_temp_node->next;
        }

        syntax_tree_node_list_node_t * equal_node = NULL;
        if (
            inner_temp_node != decl_var_decl->nonterminal.tree.tail &&
            inner_temp_node->nonterminal.nonterminal == NT_DECL_VAR_DECL_EQUAL
        ) {
            log_printf("Got equality\n");

            equal_node = inner_temp_node;
            inner_temp_node = inner_temp_node->next;
        }

        type_checker_type_t * decl_type = root_type;

        if (array_node != NULL) {
            log_printf("Adding array entry\n");

            type_checker_registry_entry_t * entry = pkcc_alloc(sizeof(type_checker_registry_entry_t));
            tr->entries[tr->size++] = entry;
            if (tr->size == tr->capacity) {
                tr->capacity *= 2;

                tr->entries = pkcc_realloc(tr->entries, tr->capacity * sizeof(type_checker_registry_entry_t *));
            }

            entry->scope = decl_var_decl->scope;
            entry->type.is_base = false;
            entry->type.derived_type.type = DTT_ARRAY;
            entry->type.derived_type.array.has_size = false; // TODO: add size info
            entry->type.derived_type.array.subtype = decl_type;
            decl_type = &entry->type;
        }

        if (equal_node != NULL) {
            equal_node->nonterminal.tree.head->next->next->desired_type = decl_type;
        }

        decl_var_decl->type = decl_type;

        const char * variable_name = token_buffer->tokens[decl_identifier_node->terminal.position].identifier_data->name;

        type_checker_variable_t * variable = type_checker_variable_registry_lookup_clashes(vr, variable_name, decl_var_decl->scope);

        if (variable == NULL) {
            log_printf("Previous definition not found\n");

            variable = type_checker_variable_registry_add(vr);
            variable->scope = decl_var_decl->scope;
            variable->type = decl_type;
            variable->name = token_buffer->tokens[decl_identifier_node->terminal.position].identifier_data->name;
            variable->defined = false;
        }
        else {
            log_printf("Previous definition found\n");

            log_printf("TEST: %s, %s\n", type_checker_type_stringify(variable->type), type_checker_type_stringify(decl_type));

            if (!type_checker_type_equal(variable->type, decl_type)) {
                fatal_line_full_error(
                    line_buffer,
                    token_buffer->tokens[decl_var_decl->nonterminal.tree.head->next->terminal.position].file_name,
                    "Conflicting definition types",
                    token_buffer->tokens[decl_var_decl->nonterminal.tree.head->next->terminal.position].line_index
                );
            }
        }

        if (equal_node != NULL) {
            if (variable->defined) {
                fatal_line_full_error(
                    line_buffer,
                    token_buffer->tokens[decl_var_decl->nonterminal.tree.head->next->terminal.position].file_name,
                    "Variable redefinition",
                    token_buffer->tokens[decl_var_decl->nonterminal.tree.head->next->terminal.position].line_index
                );
            }

            variable->defined = true;
        }
        else variable->defined = false;

        if (temp_node->next == name_list_node->nonterminal.tree.tail) name_list_node = NULL;
        else name_list_node = temp_node->next;
    }

    return NULL;
}