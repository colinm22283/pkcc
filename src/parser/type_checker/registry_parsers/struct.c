#include <parser/type_checker/type_registry.h>
#include <parser/type_checker/registry_parsers/struct.h>

#include <debug/line_error.h>

#include <alloc.h>

type_checker_type_t * type_checker_registry_parse_struct(
    type_checker_registry_t * tr,
    line_buffer_t * line_buffer,
    token_buffer_t * token_buffer,
    syntax_tree_node_list_node_t * struct_node
) {
    if (struct_node->token_type != RT_NONTERMINAL) fatal_error("Internal: Invalid definition node type!\n");
    if (struct_node->nonterminal.nonterminal != NT_STRUCT) fatal_error("Internal: Definition is not struct!\n");

    syntax_tree_node_list_node_t * struct_name_node = struct_node->nonterminal.tree.head->next->next;
    syntax_tree_node_list_node_t * struct_member_list_node;

    bool already_declared = false;
    type_checker_registry_entry_t * entry = pkcc_alloc(sizeof(type_checker_registry_entry_t));
    entry->type.is_base = false;
    entry->type.derived_type.type = DTT_STRUCT;

    if (
        struct_name_node->token_type == RT_NONTERMINAL &&
        struct_name_node->nonterminal.nonterminal == NT_STRUCT_NAME
    ) {
        syntax_tree_node_list_node_t * name_node = struct_name_node->nonterminal.tree.head->next;
        if (name_node->token_type != RT_TERMINAL) fatal_error("Internal: Invalid token type for STRUCT_NAME identifier\n");
        if (name_node->terminal.terminal != token_number_identifier()) fatal_error("Internal: Bad nonterminal for STRUCT_NAME identifier\n");

        const char * struct_name = token_buffer->tokens[name_node->terminal.position].identifier_data->name;

        log_printf("Got struct with name: %s\n", struct_name);

        for (size_t i = 0; i < tr->size; i++) {
            if (tr->entries[i] != entry) {
                if (!tr->entries[i]->type.is_base) {
                    if (tr->entries[i]->type.derived_type.type == DTT_STRUCT) {
                        if (tr->entries[i]->type.derived_type.structure.name != NULL) {
                            if (strcmp(tr->entries[i]->type.derived_type.structure.name, struct_name) == 0) {
                                log_printf("Struct already defined\n");

                                already_declared = true;

                                pkcc_free(entry);
                                entry = tr->entries[i];

                                break;
                            }
                        }
                    }
                }
            }
        }

        struct_member_list_node = struct_name_node->next;

        if (!already_declared) {
            entry->type.derived_type.structure.name = pkcc_alloc(strlen(struct_name) + 1);
            strcpy(entry->type.derived_type.structure.name, struct_name);

            tr->entries[tr->size++] = entry;
            if (tr->size == tr->capacity) {
                tr->capacity *= 2;

                tr->entries = pkcc_realloc(tr->entries, tr->capacity * sizeof(type_checker_registry_entry_t *));
            }
        }
    }
    else {
        entry->type.derived_type.structure.name = NULL;

        log_printf("Got nameless struct\n");

        struct_member_list_node = struct_name_node;

        tr->entries[tr->size++] = entry;
        if (tr->size == tr->capacity) {
            tr->capacity *= 2;

            tr->entries = pkcc_realloc(tr->entries, tr->capacity * sizeof(type_checker_registry_entry_t *));
        }
    }

    if (
        struct_member_list_node->token_type == RT_NONTERMINAL &&
        struct_member_list_node->nonterminal.nonterminal == NT_STRUCT_MEMBER_LIST
    ) {
        log_printf("Got member list\n");

        if (already_declared && entry->type.derived_type.structure.subtypes != NULL) {
            fatal_line_full_error(
                line_buffer,
                token_buffer->tokens[struct_node->nonterminal.tree.head->next->terminal.position].file_name,
                "Struct defined multiple times",
                token_buffer->tokens[struct_node->nonterminal.tree.head->next->terminal.position].line_index,
                token_buffer->tokens[struct_node->nonterminal.tree.head->next->terminal.position].position
            );
        }


        size_t member_type_count = 0, member_type_capacity = 2;
        type_checker_type_t ** member_types = pkcc_alloc(member_type_capacity * sizeof(type_checker_type_t *));

        if (struct_member_list_node->nonterminal.tree.head->next->next->next != struct_member_list_node->nonterminal.tree.tail) {
            syntax_tree_node_list_node_t * member_node = struct_member_list_node->nonterminal.tree.head->next->next;

            while (true) {
                if (member_node->token_type != RT_NONTERMINAL) fatal_error("Internal: Struct member not nonterminal\n");
                if (member_node->nonterminal.nonterminal != NT_STRUCT_MEMBER) fatal_error("Internal: Struct member not correct nonterminal\n");

                syntax_tree_node_list_node_t * member_decl_node = member_node->nonterminal.tree.head->next;
                if (member_decl_node->token_type != RT_NONTERMINAL) fatal_error("Internal: Struct member decl not nonterminal\n");
                if (member_decl_node->nonterminal.nonterminal != NT_DECL_VAR) fatal_error("Internal: Struct member decl not correct nonterminal\n");

                syntax_tree_node_list_node_t * member_decl_type_node = member_decl_node->nonterminal.tree.head->next;
                if (member_decl_type_node->token_type != RT_NONTERMINAL) fatal_error("Internal: Struct member type not nonterminal\n");
                if (member_decl_type_node->nonterminal.nonterminal != NT_TYPE) fatal_error("Internal: Struct member type not correct nonterminal\n");

                type_checker_type_t * type = member_decl_type_node->type;

                    member_types[member_type_count++] = type;

                if (member_type_count == member_type_capacity) {
                    member_type_capacity *= 2;

                    member_types = pkcc_realloc(member_types, member_type_capacity * sizeof(type_checker_type_t *));
                }

                if (member_decl_node->next->next != member_node->nonterminal.tree.tail) {
                    member_node = member_decl_node->next->next;
                }
                else break;
            }
        }

        entry->type.derived_type.structure.subtypes = member_types;
        entry->type.derived_type.structure.subtype_count = member_type_count;
    }
    else {
        log_printf("Got no member list\n");

        if (!already_declared) {
            entry->type.derived_type.structure.subtypes = NULL;
            entry->type.derived_type.structure.subtype_count = 0;
        }
    }

    // TODO: check for redeclaration

    return &entry->type;
}