#include <lexer/lexer.h>

void lexer_init(lexer_t * lexer, scanner_t * scanner) {
    lexer->token_buffer = &scanner->tb;
}

void lexer_free(lexer_t * lexer) { }

void lexer_run(lexer_t * lexer) {
    syntax_tree_parse(&lexer->syntax_tree, lexer->token_buffer);
}