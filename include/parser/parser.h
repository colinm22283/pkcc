#pragma once

#include <scanner/scanner.h>

#include <parser/parse_graph.h>
#include <parser/syntax_tree.h>

typedef struct {
    parse_graph_t parse_graph;
    syntax_tree_t syntax_tree;
} parser_t;

void parser_init(parser_t * lexer, scanner_t * scanner, line_buffer_t * line_buffer);
void parser_free(parser_t * lexer);

void parser_run(parser_t * lexer);
