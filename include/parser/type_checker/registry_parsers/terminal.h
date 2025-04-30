#pragma once

#include <parser/type_checker/type.h>
#include <parser/type_checker/type_registry.h>

type_checker_type_t * type_checker_registry_parse_terminal(
    type_checker_registry_t * tr,
    token_buffer_t * token_buffer,
    syntax_tree_node_list_node_t * terminal_node
);