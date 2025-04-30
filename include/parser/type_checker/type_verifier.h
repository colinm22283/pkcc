#pragma once

#include <parser/syntax_tree.h>

void type_verifier_run(line_buffer_t * line_buffer, token_buffer_t * token_buffer, syntax_tree_t * syntax_tree);