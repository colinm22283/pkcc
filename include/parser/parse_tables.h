#pragma once

#include <stddef.h>
#include <limits.h>
#include <stdint.h>

#include <parser/rule.h>

#define NULL_STATE (SIZE_MAX)

typedef struct {
	size_t element_count;
	token_number_t * elements;
} parse_tables_first_node_t;

typedef struct {
	nonterminal_t nonterminal;

	size_t token_count;
	rule_token_t * tokens;
} parse_tables_follow_node_t;

typedef struct {
	size_t position;

    size_t next;

    size_t lookahead_count;
    rule_token_t * lookaheads;

	rule_t * rule;
} parse_state_production_t;

typedef enum {
    AT_REDUCE,
    AT_SHIFT,
} parse_state_action_type_t;

typedef struct {
    parse_state_action_type_t type;

    size_t lookahead_count;
    rule_token_t * lookaheads;

    union {
        struct {
            nonterminal_t nonterminal;

            size_t pop_count;
        } reduce;

        struct {
            rule_token_t * token;

            size_t next_state;
        } shift;
    };
} parse_state_action_t;

typedef struct parse_state_s {
	size_t index;

	size_t production_count;
	parse_state_production_t ** productions;

	size_t follow_node_count;
	parse_tables_follow_node_t * follow_nodes;

	size_t action_count;
	parse_state_action_t * actions;
} parse_state_t;

typedef struct {
  	size_t first_node_count;
    parse_tables_first_node_t * first_nodes;

	size_t state_count;
	size_t state_capacity;
	parse_state_t ** parse_states;
} parse_tables_t;

static inline bool parse_action_lookahead_contains(parse_state_action_t * action, token_number_t lookahead) {
    if (action->lookahead_count == 0) return true;

    for (size_t i = 0; i < action->lookahead_count; i++) {
        if (action->lookaheads[i].type == RT_TERMINAL && action->lookaheads[i].terminal == lookahead) return true;
    }

    return false;
}

void parse_tables_init(parse_tables_t * parse_tables);
void parse_tables_free(parse_tables_t * parse_tables);

void parse_tables_load(parse_tables_t * parse_tables);

void parse_tables_print(parse_tables_t * parse_tables);

parse_tables_follow_node_t * parse_table_follow_lookup(parse_state_t * state, nonterminal_t nt);

