#include <parser/parser.h>

void parser_init(parser_t * parser, scanner_t * scanner) {
    syntax_tree_init(&parser->syntax_tree, &scanner->tb);
}

void parser_free(parser_t * parser) {
    syntax_tree_free(&parser->syntax_tree);
}

void parser_run(parser_t * parser) {
    syntax_tree_parse(&parser->syntax_tree);
}