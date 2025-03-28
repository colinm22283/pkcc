#pragma once

#include <scanner/scanner.h>

#include <parser/syntax_tree.h>

typedef struct {
    syntax_tree_t syntax_tree;
} parser_t;

void parser_init(parser_t * lexer, scanner_t * scanner);
void parser_free(parser_t * lexer);

void parser_run(parser_t * lexer);
