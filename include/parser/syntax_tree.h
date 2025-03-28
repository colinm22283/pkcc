#pragma once

#include <stddef.h>

#include <scanner/token_buffer.h>

#include <parser/rule.h>

struct syntax_tree_node_list_node_s;

typedef struct syntax_tree_node_s {
    struct syntax_tree_node_list_node_s * head;
    struct syntax_tree_node_list_node_s * tail;
} syntax_tree_node_t;

typedef struct syntax_tree_node_list_node_s {
    rule_token_type_t token_type;

    union {
        struct {
            nonterminal_t nonterminal;
            syntax_tree_node_t tree;
            size_t position;
        } nonterminal;

        struct {
            token_number_t terminal;
            size_t position;
        } terminal;
    };

    struct syntax_tree_node_list_node_s * prev;
    struct syntax_tree_node_list_node_s * next;
} syntax_tree_node_list_node_t;

typedef struct {
    syntax_tree_node_t head;

    token_buffer_t * token_buffer;
} syntax_tree_t;

void syntax_tree_init(syntax_tree_t * syntax_tree, token_buffer_t * token_buffer);
void syntax_tree_free(syntax_tree_t * syntax_tree);

void syntax_tree_parse(syntax_tree_t * syntax_tree);

void syntax_tree_print(syntax_tree_t * syntax_tree);

void syntax_tree_node_init(syntax_tree_node_t * node);
void syntax_tree_node_free(syntax_tree_node_t * node);
void syntax_tree_node_clear(syntax_tree_node_t * node);

void syntax_tree_node_list_link_back(syntax_tree_node_t * node, syntax_tree_node_list_node_t * list_node);
