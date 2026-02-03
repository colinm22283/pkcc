#include <parser/parser.h>

#include <debug/log.h>

void parser_init(parser_t * parser, scanner_t * scanner, line_buffer_t * line_buffer) {
    log_printf("Initializing parser\n");

    parse_tables_init(&parser->parse_tables);

    syntax_tree_init(&parser->syntax_tree, line_buffer, &scanner->tb);
}

void parser_free(parser_t * parser) {
    log_printf("Freeing parser\n");

    syntax_tree_free(&parser->syntax_tree);

    parse_tables_free(&parser->parse_tables);
}

void parser_run(parser_t * parser) {
    parse_tables_load(&parser->parse_tables);

    if (options.dump_parse_tables) parse_tables_print(&parser->parse_tables);

    syntax_tree_parse(&parser->syntax_tree, &parser->parse_tables);

    log_printf("Input accepted!\n");
}
