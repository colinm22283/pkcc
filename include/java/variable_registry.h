#pragma once

#include <stddef.h>
#include <stdio.h>

#include <parser/syntax_tree_traverser.h>
#include <parser/type_checker/type.h>
#include <parser/type_checker/scope.h>

struct java_code_generator_s;

typedef enum {
    JVR_STATIC,
    JVR_LOCAL,
    JVR_FUNCTION,
    JVR_ARRAY_REF,
} java_variable_registry_node_type_t;

typedef struct java_variable_registry_node_s {
    char * name;

    java_variable_registry_node_type_t type;

    size_t local_var_num;

    syntax_tree_traverser_t array_ref_index_stt;
    struct java_variable_registry_node_s * array_ref_parent_variable;

    type_checker_type_t * var_type;
    type_checker_scope_t * var_scope;
} java_variable_registry_node_t;

typedef struct {
    size_t size, capacity;
    java_variable_registry_node_t ** data;

    size_t current_local_var_num;
} java_variable_registry_t;

void java_variable_registry_init(java_variable_registry_t * jvr);
void java_variable_registry_free(java_variable_registry_t * jvr);

void java_variable_registry_push_static(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_type_t * type,
    type_checker_scope_t * scope
);
java_variable_registry_node_t * java_variable_registry_push_local(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_type_t * type,
    type_checker_scope_t * scope
);
java_variable_registry_node_t * java_variable_registry_push_local_number(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_type_t * type,
    type_checker_scope_t * scope,
    size_t number
);
void java_variable_registry_push_function(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_type_t * type
);
java_variable_registry_node_t * java_variable_registry_push_array_ref(
    java_variable_registry_t * jvr,
    java_variable_registry_node_t * parent_variable,
    syntax_tree_traverser_t index_expression_stt
);

java_variable_registry_node_t * java_variable_registry_lookup(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_scope_t * scope
);

void java_variable_registry_load(
    struct java_code_generator_s * jcg,
    java_variable_registry_node_t * node
);

void java_variable_registry_store(
    struct java_code_generator_s * jcg,
    java_variable_registry_node_t * node
);

void java_variable_registry_call(
    FILE * out_file,
    const char * classname,
    java_variable_registry_node_t * node
);