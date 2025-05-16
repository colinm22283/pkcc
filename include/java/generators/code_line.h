#pragma once

#include <parser/syntax_tree_traverser.h>

#include <java/code_generator.h>

java_variable_registry_node_t * java_generator_code_line(
    java_code_generator_t * jcg,
    syntax_tree_traverser_t stt,
    type_checker_type_t * function_type,
    java_label_t * break_label,
    java_label_t * continue_label
);