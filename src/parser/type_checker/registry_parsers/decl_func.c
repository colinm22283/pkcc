#include <parser/type_checker/error.h>
#include <parser/type_checker/registry_parsers/decl_func.h>

#include <debug/line_error.h>

void type_checker_registry_parse_code_block_recur(
    line_buffer_t * line_buffer,
    token_buffer_t * token_buffer,
    type_checker_type_t * function_type,
    syntax_tree_node_t * node
) {
    syntax_tree_node_list_node_t * subnode = node->head->next;

    while (subnode != node->tail) {
        if (subnode->token_type == RT_NONTERMINAL) {
            if (subnode->nonterminal.nonterminal == NT_STATEMENT) {
                syntax_tree_node_list_node_t * subsubnode = subnode->nonterminal.tree.head->next;

                if (
                    subsubnode->token_type == RT_TERMINAL &&
                    subsubnode->terminal.terminal == token_number_keyword(SCANNER_KEYWORD_TYPE_RETURN)
                ) {
                    syntax_tree_node_list_node_t * return_value_node = subsubnode->next;

                    if (return_value_node->token_type == RT_NONTERMINAL) {
                        return_value_node->nonterminal.tree.head->next->desired_type = function_type->derived_type.function.return_type;
                    }
                    else {
                        if (!type_checker_type_equal(function_type->derived_type.function.return_type, void_type)) {
                            fatal_line_full_error(
                                line_buffer,
                                token_buffer->tokens[subsubnode->terminal.position].file_name,
                                "No return value specified for non-void function",
                                token_buffer->tokens[subsubnode->terminal.position].line_index
                            );
                        }
                    }
                }
            }

            type_checker_registry_parse_code_block_recur(line_buffer, token_buffer, function_type, &subnode->nonterminal.tree);
        }

        subnode = subnode->next;
    }
}

type_checker_type_t * type_checker_registry_parse_decl_func(
    type_checker_registry_t * tr,
    type_checker_variable_registry_t * vr,
    line_buffer_t * line_buffer,
    token_buffer_t * token_buffer,
    syntax_tree_node_list_node_t * function_node
) {
    if (function_node->token_type != RT_NONTERMINAL) type_checker_error();
    if (function_node->nonterminal.nonterminal != NT_DECL_FUNC) type_checker_error();

    log_printf("Function\n");

    syntax_tree_node_list_node_t * temp_node = function_node->nonterminal.tree.head->next;

    syntax_tree_node_list_node_t * return_type_node = temp_node;
    temp_node = temp_node->next;

    syntax_tree_node_list_node_t * identifier_node = temp_node;
    temp_node = temp_node->next;

    temp_node = temp_node->next;

    syntax_tree_node_list_node_t * arg_list_node = NULL;
    if (temp_node->token_type == RT_NONTERMINAL) {
        arg_list_node = temp_node;
        temp_node = temp_node->next;
    }
    temp_node = temp_node->next;

    syntax_tree_node_list_node_t * code_block_node = NULL;
    if (
        temp_node != function_node->nonterminal.tree.tail &&
        temp_node->token_type == RT_NONTERMINAL
    ) {
        code_block_node = temp_node;
    }

    size_t arg_count = 0, arg_capacity = 1;
    type_checker_type_t ** args = pkcc_alloc(arg_capacity * sizeof(type_checker_type_t *));

    if (arg_list_node != NULL) {
        syntax_tree_node_list_node_t * inner_temp_node = arg_list_node->nonterminal.tree.head->next;
        syntax_tree_node_list_node_t * tail = arg_list_node->nonterminal.tree.tail;

        while (true) {
            size_t arg_index = arg_count++;

            args[arg_index] = inner_temp_node->type;
            inner_temp_node = inner_temp_node->next;

            syntax_tree_node_list_node_t * arg_identifier_node = inner_temp_node;
            inner_temp_node = inner_temp_node->next;

            if (arg_count == arg_capacity) {
                arg_capacity *= 2;

                args = pkcc_realloc(args, arg_capacity * sizeof(type_checker_type_t *));
            }

            syntax_tree_node_list_node_t * array_node = NULL;
            if (
                inner_temp_node != function_node->nonterminal.tree.tail &&
                inner_temp_node->token_type == RT_NONTERMINAL &&
                inner_temp_node->nonterminal.nonterminal == NT_DECL_VAR_DECL_ARRAY
            ) {
                log_printf("Argument is array\n");

                array_node = inner_temp_node;
                inner_temp_node = inner_temp_node->next;
            }

            if (array_node != NULL) {
                log_printf("Adding array entry\n");
                type_checker_registry_entry_t * entry = pkcc_alloc(sizeof(type_checker_registry_entry_t));
                tr->entries[tr->size++] = entry;
                if (tr->size == tr->capacity) {
                    tr->capacity *= 2;

                    tr->entries = pkcc_realloc(tr->entries, tr->capacity * sizeof(type_checker_registry_entry_t *));
                }

                entry->scope = function_node->scope;
                entry->type.is_base = false;
                entry->type.derived_type.type = DTT_ARRAY;
                entry->type.derived_type.array.has_size = false; // TODO: add size info
                entry->type.derived_type.array.subtype = args[arg_index];
                args[arg_index] = &entry->type;
            }

            const char * variable_name = token_buffer->tokens[arg_identifier_node->terminal.position].identifier_data->name;

            log_printf("Got argument '%s'\n", variable_name);

            type_checker_variable_t * variable = type_checker_variable_registry_lookup_clashes(vr, variable_name, function_node->scope);

            if (variable == NULL) {
                log_printf("Previous definition not found\n");

                variable = type_checker_variable_registry_add(vr);
                variable->scope = arg_identifier_node->scope;
                variable->type = args[arg_index];
                variable->name = variable_name;
                variable->defined = true;
            }
            else {
                log_printf("Previous definition found\n");

                if (variable->defined) {
                    fatal_line_full_error(
                        line_buffer,
                        token_buffer->tokens[function_node->nonterminal.position].file_name,
                        "Variable redefinition",
                        token_buffer->tokens[function_node->nonterminal.position].line_index
                    );
                }

                if (!type_checker_type_equal(variable->type, args[arg_index])) {
                    fatal_line_full_error(
                        line_buffer,
                        token_buffer->tokens[function_node->nonterminal.position].file_name,
                        "Conflicting definitions",
                        token_buffer->tokens[function_node->nonterminal.position].line_index
                    );
                }
            }

            if (
                tail->prev->token_type == RT_NONTERMINAL &&
                tail->prev->nonterminal.nonterminal == NT_DECL_FUNC_ARG_LIST_EXTRA
            ) {
                inner_temp_node = tail->prev->nonterminal.tree.head->next->next;
                tail = tail->prev->nonterminal.tree.tail;
            }
            else break;
        }
    }

    log_printf("Got function with %zu args\n", arg_count);

    type_checker_registry_entry_t * entry = pkcc_alloc(sizeof(type_checker_registry_entry_t));
    tr->entries[tr->size++] = entry;
    if (tr->size == tr->capacity) {
        tr->capacity *= 2;

        tr->entries = pkcc_realloc(tr->entries, tr->capacity * sizeof(type_checker_registry_entry_t *));
    }
    entry->scope = function_node->scope;
    entry->type.is_base = false;
    entry->type.derived_type.type = DTT_FUNCTION;
    entry->type.derived_type.function.arg_count = arg_count;
    entry->type.derived_type.function.args = args;
    entry->type.derived_type.function.return_type = return_type_node->type;

    const char * variable_name = token_buffer->tokens[identifier_node->terminal.position].identifier_data->name;

    type_checker_variable_t * variable = type_checker_variable_registry_lookup_clashes(vr, variable_name, function_node->scope);

    if (variable == NULL) {
        log_printf("Previous definition not found\n");

        variable = type_checker_variable_registry_add(vr);
        variable->scope = function_node->scope;
        variable->type = &entry->type;
        variable->name = token_buffer->tokens[identifier_node->terminal.position].identifier_data->name;
        variable->defined = false;
    }
    else {
        log_printf("Previous definition found\n");

        if (!type_checker_type_equal(variable->type, &entry->type)) {
            fatal_line_full_error(
                line_buffer,
                token_buffer->tokens[function_node->nonterminal.position].file_name,
                "Conflicting definitions",
                token_buffer->tokens[function_node->nonterminal.position].line_index
            );
        }
    }

    if (code_block_node != NULL) {
        if (variable->defined) {
            fatal_line_full_error(
                line_buffer,
                token_buffer->tokens[function_node->nonterminal.position].file_name,
                "Variable redefinition",
                token_buffer->tokens[function_node->nonterminal.position].line_index
            );
        }

        variable->defined = true;

        type_checker_registry_parse_code_block_recur(line_buffer, token_buffer, &entry->type, &code_block_node->nonterminal.tree);
    }
    else variable->defined = false;

    return &entry->type;
}