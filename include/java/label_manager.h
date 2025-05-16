#pragma once

#include <stddef.h>

typedef struct {
    char * name;
} java_label_t;

typedef struct {
    size_t current_label;

    size_t size, capacity;
    java_label_t ** labels;
} java_label_manager_t;

void java_label_manager_init(java_label_manager_t * jlm);
void java_label_manager_free(java_label_manager_t * jlm);

java_label_t * java_label_manager_add(java_label_manager_t * jlm);