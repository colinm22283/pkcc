#pragma once

#include <parser/rules.h>
#include <parser/type_checker/type_registry.h>
#include <parser/type_checker/variable_registry.h>

#include <line_buffer.h>

type_checker_type_t * type_checker_registry_parse_decl_var(
    type_checker_registry_t * tr,
    type_checker_variable_registry_t * vr,
    line_buffer_t * line_buffer,
    token_buffer_t * token_buffer,
    syntax_tree_node_list_node_t * expression_node
);