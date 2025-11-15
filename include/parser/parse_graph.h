#pragma once

typedef struct {
} parse_graph_t;

void parse_graph_init(parse_graph_t * parse_graph);
void parse_graph_free(parse_graph_t * parse_graph);

void parse_graph_load(parse_graph_t * parse_graph);
