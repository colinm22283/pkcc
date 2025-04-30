#pragma once

#include <parser/rules.h>
#include <parser/type_checker/type_registry.h>

#include <line_buffer.h>

type_checker_type_t * type_checker_registry_parse_expression(
    type_checker_registry_t * tr,
    line_buffer_t * line_buffer,
    token_buffer_t * token_buffer,
    syntax_tree_node_list_node_t * expression_node
);
