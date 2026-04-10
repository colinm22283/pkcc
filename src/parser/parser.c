#include <parser/parser.h>
#include <parser/parser_rules.h>

#include <debug/log.h>

void parser_init(parser_t * parser, scanner_t * scanner, line_buffer_t * line_buffer) {
    log_printf("Initializing parser\n");

    parse_graph_init(&parser->parse_graph);

    syntax_tree_init(&parser->syntax_tree, line_buffer, &scanner->tb);
}

void parser_free(parser_t * parser) {
    log_printf("Freeing parser\n");

    syntax_tree_free(&parser->syntax_tree);

    parse_graph_free(&parser->parse_graph);
}

void parser_run(parser_t * parser) {
    parse_graph_load(&parser->parse_graph);

    parser_rules_match(&parser->syntax_tree);
}