#include <parser/syntax_tree_traverser.h>

void syntax_tree_traverser_init(syntax_tree_traverser_t * stt, syntax_tree_node_t * head_node) {
    stt->head_node = head_node;

    syntax_tree_traverser_reset(stt);
}

void syntax_tree_traverser_reset(syntax_tree_traverser_t * stt) {
    stt->current_node = stt->head_node->head->next;
}

bool syntax_tree_traverser_next(syntax_tree_traverser_t * stt) {
    if (stt->current_node == stt->head_node->tail) return false;

    stt->current_node = stt->current_node->next;

    return true;
}

bool syntax_tree_traverser_enter(syntax_tree_traverser_t * stt, syntax_tree_traverser_t * new_stt) {
    if (stt->current_node == stt->head_node->tail) return false;

    if (stt->current_node->token_type != RT_NONTERMINAL) return false;

    syntax_tree_traverser_init(new_stt, &stt->current_node->nonterminal.tree);

    return true;
}

bool syntax_tree_traverser_enter_self(syntax_tree_traverser_t * stt) {
    if (stt->current_node == stt->head_node->tail) return false;

    if (stt->current_node->token_type != RT_NONTERMINAL) return false;

    syntax_tree_node_t * new_node = &stt->current_node->nonterminal.tree;

    syntax_tree_traverser_init(stt, new_node);

    return true;
}

bool syntax_tree_traverser_seek_terminal(syntax_tree_traverser_t * stt, token_number_t token_number) {
    syntax_tree_node_list_node_t * original_node = stt->current_node;

    while (stt->current_node != stt->head_node->tail) {
        if (stt->current_node->token_type == RT_TERMINAL) {
            if (stt->current_node->terminal.terminal == token_number) {
                return true;
            }
        }

        stt->current_node = stt->current_node->next;
    }

    stt->current_node = original_node;
    return false;
}

bool syntax_tree_traverser_seek_nonterminal(syntax_tree_traverser_t * stt, nonterminal_t nonterminal) {
    syntax_tree_node_list_node_t * original_node = stt->current_node;

    while (stt->current_node != stt->head_node->tail) {
        if (stt->current_node->token_type == RT_NONTERMINAL) {
            if (stt->current_node->nonterminal.nonterminal == nonterminal) {
                return true;
            }
        }

        stt->current_node = stt->current_node->next;
    }

    stt->current_node = original_node;
    return false;
}