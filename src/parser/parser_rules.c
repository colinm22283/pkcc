#include <parser/parser_rules.h>

#include <alloc.h>

syntax_tree_t * syn_tree;

#define TOK() &(st)->token_buffer->tokens[*pos]
#define NEXT() &(st)->token_buffer->tokens[*pos + 1]

static inline void shift(syntax_tree_node_list_node_t * stn, token_number_t tok, size_t * pos) {
    log_printf("SHIFT %s\n", token_number_stringify(tok));

    syntax_tree_node_list_node_t * new_node = pkcc_alloc(sizeof(syntax_tree_node_list_node_t));
    new_node->token_type = STNT_TERMINAL;
    new_node->terminal.terminal = tok;
    new_node->terminal.position = *pos;
    syntax_tree_node_list_link_back(&stn->nonterminal.tree, new_node);
    (*pos)++;
}

static inline syntax_tree_node_list_node_t * reduce(syntax_tree_node_list_node_t * stn, nonterminal_t nt, size_t * pos, size_t size) {
    log_printf("REDUCE %s\n", rules_nonterminal_name(nt));

    syntax_tree_node_list_node_t * new_node = pkcc_alloc(sizeof(syntax_tree_node_list_node_t));

    new_node->token_type = STNT_NONTERMINAL;
    new_node->nonterminal.nonterminal = nt;
    new_node->nonterminal.position = *pos;
    syntax_tree_node_init(&new_node->nonterminal.tree);

    syntax_tree_node_list_node_t ** nodes = pkcc_alloc(size * sizeof(syntax_tree_node_list_node_t *));

    for (size_t i = 0; i < size; i++) {
        nodes[i] = syntax_tree_node_list_unlink_back(&stn->nonterminal.tree);

        log_printf("Unlinking %s\n", rules_nonterminal_name(nodes[i]->terminal.terminal));
    }

    for (size_t i = 0; i < size; i++) {
        syntax_tree_node_list_link_back(&new_node->nonterminal.tree, nodes[size - i - 1]);
    }

    pkcc_free(nodes);
    
    return new_node;
}

static inline token_number_t peek(syntax_tree_node_list_node_t * stn) {
    return stn->nonterminal.tree.tail->prev->terminal.terminal;
}

syntax_tree_node_list_node_t * nt_type_specifier(syntax_tree_t * st, syntax_tree_node_list_node_t * stn, size_t * pos) {
    log_printf("Peek: %s\n", token_number_stringify(peek(stn)));

    switch (peek(stn)) {
        case token_number_keyword(SCANNER_KEYWORD_TYPE_INT):
        case token_number_keyword(SCANNER_KEYWORD_TYPE_CHAR): return reduce(stn, NT_TYPE_SPECIFIER, pos, 1); break;

        default: return stn;
    }
}

syntax_tree_node_list_node_t * nt_external_declaration(syntax_tree_t * st, syntax_tree_node_list_node_t * stn, size_t * pos) {
    token_number_t tok = token_number(TOK());

    return stn;
}

syntax_tree_node_list_node_t * nt_translation_unit(syntax_tree_t * st, syntax_tree_node_list_node_t * stn, size_t * pos) {
    while (*pos != st->token_buffer->token_count) {
        token_number_t tok = token_number(TOK());

        switch (tok) {
            case token_number_keyword(SCANNER_KEYWORD_TYPE_INT):
            case token_number_keyword(SCANNER_KEYWORD_TYPE_CHAR): {
                shift(stn, tok, pos); stn = nt_type_specifier(st, stn, pos);
            } break;

            case token_number_keyword(SCANNER_KEYWORD_TYPE_STATIC): {
                shift(stn, tok, pos); stn = nt_type_specifier(st, stn, pos);
            } break;

            default: return stn;
        }
    }

    return stn;
}

void parser_rules_match(syntax_tree_t * st) {
    syn_tree = st;

    size_t pos = 0;

    syntax_tree_node_list_node_t * new_node = pkcc_alloc(sizeof(syntax_tree_node_list_node_t));
    new_node->token_type = STNT_NONTERMINAL;
    new_node->nonterminal.nonterminal = NT_TRANSLATION_UNIT;
    new_node->nonterminal.position = 0;
    syntax_tree_node_init(&new_node->nonterminal.tree);

    syntax_tree_node_list_link_back(&st->head, nt_translation_unit(st, new_node, &pos));
}
