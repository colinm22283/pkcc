#pragma once

#include <parser/syntax_tree_traverser.h>

#include <java/code_generator.h>

java_variable_registry_node_t * java_generator_local_var(
    java_code_generator_t * jcg,
    syntax_tree_traverser_t stt,
    type_checker_type_t * function_type
);