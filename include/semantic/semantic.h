#pragma once

#include <parser/parser.h>

typedef struct {
    syntax_tree_t * ast;
} semantic_t;

void semantic_init(semantic_t * semantic, parser_t * parser);
void semantic_free(semantic_t * semantic);

void semantic_run(semantic_t * semantic);

void semantic_typed_tree_print(semantic_t * semantic);
