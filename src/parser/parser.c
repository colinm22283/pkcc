#include <parser/parser.h>

#include <debug/log.h>

void parser_init(parser_t * parser, scanner_t * scanner, line_buffer_t * line_buffer) {
    log_printf("Initializing parser\n");

    syntax_tree_init(&parser->syntax_tree, &scanner->tb);

    type_checker_init(&parser->type_checker, line_buffer, &parser->syntax_tree, &scanner->tb);
}

void parser_free(parser_t * parser) {
    log_printf("Freeing parser\n");

    type_checker_free(&parser->type_checker);

    syntax_tree_free(&parser->syntax_tree);
}

void parser_run(parser_t * parser) {
    syntax_tree_parse(&parser->syntax_tree);

    if (options.dump_tree) syntax_tree_print(&parser->syntax_tree);

    type_checker_run(&parser->type_checker);
}