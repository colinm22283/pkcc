#include <java/variable_registry.h>
#include <java/java_type_stringify.h>
#include <java/java_prefix.h>
#include <java/code_generator.h>
#include <java/generators/expression.h>

#include <alloc.h>

void java_variable_registry_init(java_variable_registry_t * jvr) {
    jvr->size = 0;
    jvr->capacity = 4;
    jvr->data = pkcc_alloc(jvr->capacity * sizeof(java_variable_registry_node_t *));

    jvr->current_local_var_num = 10;
}

void java_variable_registry_free(java_variable_registry_t * jvr) {
    for (size_t i = 0; i < jvr->size; i++) {
        pkcc_free(jvr->data[i]->name);
        pkcc_free(jvr->data[i]);
    }

    pkcc_free(jvr->data);
}

void java_variable_registry_push_static(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_type_t * type,
    type_checker_scope_t * scope
) {
    jvr->data[jvr->size] = pkcc_alloc(sizeof(java_variable_registry_node_t));
    java_variable_registry_node_t * new_node = jvr->data[jvr->size++];

    new_node->name = pkcc_alloc(strlen(name) + 1);
    strcpy(new_node->name, name);

    new_node->type = JVR_STATIC;
    new_node->var_type = type;
    new_node->var_scope = scope;

    if (jvr->size == jvr->capacity) {
        jvr->capacity *= 2;

        jvr->data = pkcc_realloc(jvr->data, jvr->capacity * sizeof(java_variable_registry_node_t *));
    }
}

java_variable_registry_node_t * java_variable_registry_push_local(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_type_t * type,
    type_checker_scope_t * scope
) {
    jvr->data[jvr->size] = pkcc_alloc(sizeof(java_variable_registry_node_t));
    java_variable_registry_node_t * new_node = jvr->data[jvr->size++];

    new_node->name = pkcc_alloc(strlen(name) + 1);
    strcpy(new_node->name, name);

    new_node->type = JVR_LOCAL;
    new_node->var_type = type;
    new_node->var_scope = scope;

    new_node->local_var_num = jvr->current_local_var_num++;

    if (jvr->size == jvr->capacity) {
        jvr->capacity *= 2;

        jvr->data = pkcc_realloc(jvr->data, jvr->capacity * sizeof(java_variable_registry_node_t *));
    }

    return new_node;
}

java_variable_registry_node_t * java_variable_registry_push_local_number(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_type_t * type,
    type_checker_scope_t * scope,
    size_t number
) {
    jvr->data[jvr->size] = pkcc_alloc(sizeof(java_variable_registry_node_t));
    java_variable_registry_node_t * new_node = jvr->data[jvr->size++];

    new_node->name = pkcc_alloc(strlen(name) + 1);
    strcpy(new_node->name, name);

    new_node->type = JVR_LOCAL;
    new_node->var_type = type;
    new_node->var_scope = scope;

    new_node->local_var_num = number;

    if (jvr->size == jvr->capacity) {
        jvr->capacity *= 2;

        jvr->data = pkcc_realloc(jvr->data, jvr->capacity * sizeof(java_variable_registry_node_t *));
    }

    return new_node;
}

void java_variable_registry_push_function(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_type_t * type
) {
    for (size_t i = 0; i < jvr->size; i++) {
        if (strcmp(name, jvr->data[i]->name) == 0) return;
    }

    jvr->data[jvr->size] = pkcc_alloc(sizeof(java_variable_registry_node_t));
    java_variable_registry_node_t * new_node = jvr->data[jvr->size++];

    new_node->name = pkcc_alloc(strlen(name) + 1);
    strcpy(new_node->name, name);

    new_node->type = JVR_FUNCTION;
    new_node->var_type = type;

    if (jvr->size == jvr->capacity) {
        jvr->capacity *= 2;

        jvr->data = pkcc_realloc(jvr->data, jvr->capacity * sizeof(java_variable_registry_node_t *));
    }
}

java_variable_registry_node_t * java_variable_registry_push_array_ref(
    java_variable_registry_t * jvr,
    java_variable_registry_node_t * parent_variable,
    syntax_tree_traverser_t index_expression_stt
) {
    jvr->data[jvr->size] = pkcc_alloc(sizeof(java_variable_registry_node_t));
    java_variable_registry_node_t * new_node = jvr->data[jvr->size++];

    new_node->name = pkcc_alloc(3);
    strcpy(new_node->name, "!!");

    new_node->type = JVR_ARRAY_REF;
    new_node->var_type = parent_variable->var_type->derived_type.array.subtype;
    new_node->var_scope = parent_variable->var_scope;

    new_node->array_ref_index_stt = index_expression_stt;
    new_node->array_ref_parent_variable = parent_variable;

    if (jvr->size == jvr->capacity) {
        jvr->capacity *= 2;

        jvr->data = pkcc_realloc(jvr->data, jvr->capacity * sizeof(java_variable_registry_node_t *));
    }

    return new_node;
}

java_variable_registry_node_t * java_variable_registry_lookup(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_scope_t * scope
) {
    java_variable_registry_node_t * node = NULL;

    for (size_t i = 0; i < jvr->size; i++) {
        if (strcmp(jvr->data[i]->name, name) == 0) {
            if (jvr->data[i]->type == JVR_FUNCTION) {
                return jvr->data[i];
            }
            else {
                if (type_checker_scope_is_parent(jvr->data[i]->var_scope, scope)) {
                    if (node == NULL) node = jvr->data[i];
                    else {
                        if (jvr->data[i]->var_scope->depth > node->var_scope->depth) {
                            node = jvr->data[i];
                        }
                    }
                }
            }
        }
    }

    return node;
}

void java_variable_registry_load(
    java_code_generator_t * jcg,
    java_variable_registry_node_t * node
) {
    switch (node->type) {
        case JVR_STATIC: {
            char * type_str = java_type_stringify(node->var_type);

            fprintf(
                jcg->out_file,
                "\t\tgetstatic %s/%s %s\n",
                jcg->classname,
                node->name,
                type_str
            );

            pkcc_free(type_str);
        } break;

        case JVR_LOCAL: {
            fprintf(
                jcg->out_file,
                "\t\t%cload %zu\n",
                java_prefix_reduced(node->var_type),
                node->local_var_num
            );
        } break;

        case JVR_ARRAY_REF: {
            java_variable_registry_load(
                jcg,
                node->array_ref_parent_variable
            );

            java_generator_expression(jcg, node->array_ref_index_stt);

            fprintf(
                jcg->out_file,
                "\t\t%caload\n",
                java_prefix(node->var_type)
            );
        } break;

        default: {
            fatal_error("Cannot load function as variable\n");
        } break;
    }
}

void java_variable_registry_store(
    java_code_generator_t * jcg,
    java_variable_registry_node_t * node
) {
    switch (node->type) {
        case JVR_STATIC: {
            char * type_str = java_type_stringify(node->var_type);

            fprintf(
                jcg->out_file,
                "\t\tputstatic %s/%s %s\n",
                jcg->classname,
                node->name,
                type_str
            );

            pkcc_free(type_str);
        } break;

        case JVR_LOCAL: {
            fprintf(
                jcg->out_file,
                "\t\t%cstore %zu\n",
                java_prefix_reduced(node->var_type),
                node->local_var_num
            );
        } break;

        case JVR_ARRAY_REF: {
            fprintf(
                jcg->out_file,
                "\t\t%cstore 255\n",
                java_prefix_reduced(node->var_type)
            );

            java_variable_registry_load(
                jcg,
                node->array_ref_parent_variable
            );

            java_generator_expression(jcg, node->array_ref_index_stt);

            fprintf(
                jcg->out_file,
                "\t\t%cload 255\n"
                "\t\t%castore\n",
                java_prefix_reduced(node->var_type),
                java_prefix(node->var_type)
            );
        } break;

        default: {
            fatal_error("Cannot store function as variable\n");
        } break;
    }
}

void java_variable_registry_call(
    FILE * out_file,
    const char * classname,
    java_variable_registry_node_t * node
) {
    if (node->type != JVR_FUNCTION) fatal_error("Cannot call non function variable\n");

    char * type_str = java_type_stringify(node->var_type);

    if (
        strcmp(node->name, "getchar") == 0 ||
        strcmp(node->name, "putchar") == 0 ||
        strcmp(node->name, "getint") == 0 ||
        strcmp(node->name, "putint") == 0 ||
        strcmp(node->name, "getfloat") == 0 ||
        strcmp(node->name, "putfloat") == 0 ||
        strcmp(node->name, "putstring") == 0
    ) classname = "lib440";

    fprintf(
        out_file,
        "\t\tinvokestatic Method %s %s %s\n",
        classname,
        node->name,
        type_str
    );

    pkcc_free(type_str);
}