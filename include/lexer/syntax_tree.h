#pragma once

#include <stddef.h>

#include <scanner/token_buffer.h>

typedef struct syntax_tree_node_s {
    size_t node_count, node_capacity;
    struct syntax_tree_node_s * nodes;
} syntax_tree_node_t;

typedef struct {


    syntax_tree_node_t head;
} syntax_tree_t;

void syntax_tree_init(syntax_tree_t * syntax_tree);
void syntax_tree_free(syntax_tree_t * syntax_tree);

void syntax_tree_parse(syntax_tree_t * syntax_tree, token_buffer_t * token_buffer);