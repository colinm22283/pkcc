#pragma once

#include <stddef.h>

#include <parser/type_checker/type.h>
#include <parser/syntax_tree.h>

typedef struct {
    type_checker_type_t type;
} type_checker_registry_entry_t;

typedef struct {
    size_t size, capacity;
    type_checker_registry_entry_t ** entries;
} type_checker_registry_t;

void type_checker_registry_init(type_checker_registry_t * tr);
void type_checker_registry_free(type_checker_registry_t * tr);

type_checker_type_t * type_checker_registry_parse(type_checker_registry_t * tr, token_buffer_t * token_buffer, syntax_tree_node_list_node_t * node);