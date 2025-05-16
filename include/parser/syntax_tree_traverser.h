#pragma once

#include <stdbool.h>

#include <parser/syntax_tree.h>

typedef struct {
    syntax_tree_node_t * head_node;
    syntax_tree_node_list_node_t * current_node;
} syntax_tree_traverser_t;

void syntax_tree_traverser_init(syntax_tree_traverser_t * stt, syntax_tree_node_t * head_node);
void syntax_tree_traverser_reset(syntax_tree_traverser_t * stt);

bool syntax_tree_traverser_next(syntax_tree_traverser_t * stt);
bool syntax_tree_traverser_enter(syntax_tree_traverser_t * stt, syntax_tree_traverser_t * new_stt);
bool syntax_tree_traverser_enter_self(syntax_tree_traverser_t * stt);

bool syntax_tree_traverser_seek_terminal(syntax_tree_traverser_t * stt, token_number_t token_number);
bool syntax_tree_traverser_seek_nonterminal(syntax_tree_traverser_t * stt, nonterminal_t nonterminal);