#include <semantic/semantic.h>

void semantic_init(semantic_t * semantic, parser_t * parser) {
    semantic->ast = &parser->syntax_tree;
}

void semantic_free(__MAYBE_UNUSED semantic_t * semantic) {

}

void semantic_run(__MAYBE_UNUSED semantic_t * semantic) {

}

void semantic_typed_tree_print(__MAYBE_UNUSED semantic_t * semantic) {

}