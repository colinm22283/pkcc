#pragma once

#include <parser/syntax_tree.h>
#include <parser/type_checker/type.h>
#include <parser/type_checker/type_registry.h>

typedef struct {
    syntax_tree_t * syntax_tree;
    line_buffer_t * line_buffer;
    token_buffer_t * token_buffer;

    type_checker_registry_t type_registry;
} type_checker_t;

void type_checker_init(type_checker_t * tc, line_buffer_t * line_buffer, syntax_tree_t * syntax_tree, token_buffer_t * token_buffer);
void type_checker_free(type_checker_t * tc);

void type_checker_run(type_checker_t * tc);