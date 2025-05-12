#include <java/variable_registry.h>
#include <java/java_type_stringify.h>
#include <java/java_prefix.h>

#include <alloc.h>

void java_variable_registry_init(java_variable_registry_t * jvr) {
    jvr->size = 0;
    jvr->capacity = 4;
    jvr->data = pkcc_alloc(jvr->capacity * sizeof(java_variable_registry_node_t *));

    jvr->current_local_var_num = 0;
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
}

size_t java_variable_registry_push_local(
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

    return new_node->local_var_num;
}

java_variable_registry_node_t * java_variable_registry_lookup(
    java_variable_registry_t * jvr,
    const char * name,
    type_checker_scope_t * scope
) {
    java_variable_registry_node_t * node = NULL;

    for (size_t i = 0; i < jvr->size; i++) {
        if (strcmp(jvr->data[i]->name, name) == 0) {
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

    return node;
}

void java_variable_registry_load(
    FILE * out_file,
    const char * classname,
    java_variable_registry_node_t * node
) {
    if (node->type == JVR_STATIC) {
        char * type_str = java_type_stringify(node->var_type);

        fprintf(
            out_file,
            "\t\tgetstatic %s/%s %s\n",
            classname,
            node->name,
            type_str
        );

        pkcc_free(type_str);
    }
    else {
        fprintf(
            out_file,
            "\t\t%cload %zu\n",
            java_prefix(node->var_type),
            node->local_var_num
        );
    }
}

void java_variable_registry_store(
    FILE * out_file,
    const char * classname,
    java_variable_registry_node_t * node
) {
    if (node->type == JVR_STATIC) {
        char * type_str = java_type_stringify(node->var_type);

        fprintf(
            out_file,
            "\t\tputstatic %s/%s %s\n",
            classname,
            node->name,
            type_str
        );

        pkcc_free(type_str);
    }
    else {
        fprintf(
            out_file,
            "\t\t%cstore %zu\n",
            java_prefix(node->var_type),
            node->local_var_num
        );
    }
}