#pragma once

#include <stddef.h>

#include <parser/rule.h>

typedef struct {
	size_t position;
	rule_token_t * lookahead;

	size_t rule_count;
	rule_t * rules;
} parse_graph_production_t;

typedef struct parse_graph_node_s {

} parse_graph_node_t;

typedef struct {
	size_t element_count;
    token_number_t * elements;
} parse_tables_first_node_t;

typedef struct {
  	size_t first_node_count;
    parse_tables_first_node_t * first_nodes;
} parse_tables_t;

void parse_tables_init(parse_tables_t * parse_tables);
void parse_tables_free(parse_tables_t * parse_tables);

void parse_tables_load(parse_tables_t * parse_tables);

void parse_tables_print(parse_tables_t * parse_tables);
