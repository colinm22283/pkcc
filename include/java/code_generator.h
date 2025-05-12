#pragma once

#include <stdio.h>

#include <parser/syntax_tree.h>

#include <java/variable_registry.h>

typedef struct {
    syntax_tree_t * syntax_tree;
    FILE * out_file;

    char * classname;

    java_variable_registry_t jvr;
} java_code_generator_t;

void java_code_generator_init(java_code_generator_t * jcg, FILE * out_file, syntax_tree_t * syntax_tree);
void java_code_generator_free(java_code_generator_t * jcg);

void java_code_generator_run(java_code_generator_t * jcg);