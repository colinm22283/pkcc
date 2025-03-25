#pragma once

#include <scanner/scanner.h>

#include <lexer/syntax_tree.h>

typedef struct {
    syntax_tree_t syntax_tree;

    token_buffer_t * token_buffer;
} lexer_t;

void lexer_init(lexer_t * lexer, scanner_t * scanner);
void lexer_free(lexer_t * lexer);

void lexer_run(lexer_t * lexer);
