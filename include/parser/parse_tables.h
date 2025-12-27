#pragma once

#include <stddef.h>

#include <parser/rule.h>

typedef struct {
	size_t position;

	size_t shift;

	size_t reduce;
	rule_token_t * lookahead;

	rule_t * rule;
} parse_state_production_t;

typedef struct parse_state_s {
	size_t production_count;
	parse_state_production_t ** productions;
} parse_state_t;

typedef struct {
	size_t element_count;
    token_number_t * elements;
} parse_tables_first_node_t;

typedef struct {
	size_t element_count;
	token_number_t * elements;
} parse_tables_follow_node_t;

typedef struct {
  	size_t first_node_count;
    parse_tables_first_node_t * first_nodes;

	size_t follow_node_count;
	parse_tables_follow_node_t * follow_nodes;

	size_t state_count;
	size_t state_capacity;
	parse_state_t ** parse_states;
} parse_tables_t;

void parse_tables_init(parse_tables_t * parse_tables);
void parse_tables_free(parse_tables_t * parse_tables);

void parse_tables_load(parse_tables_t * parse_tables);

void parse_tables_print(parse_tables_t * parse_tables);

