#pragma once

#include <stddef.h>
#include <stdio.h>

#include <parser/type_checker/type.h>
#include <parser/type_checker/scope.h>

typedef enum {
    JVR_STATIC,
    JVR_LOCAL,
} java_variable_registry_node_type_t;

typedef struct {
    char * name;

    java_variable_registry_node_type_t type;

    size_t local_var_num;

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
size_t java_variable_registry_push_local(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_type_t * type,
    type_checker_scope_t * scope
);

java_variable_registry_node_t * java_variable_registry_lookup(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_scope_t * scope
);

void java_variable_registry_load(
    FILE * out_file,
    const char * classname,
    java_variable_registry_node_t * node
);

void java_variable_registry_store(
    FILE * out_file,
    const char * classname,
    java_variable_registry_node_t * node
);