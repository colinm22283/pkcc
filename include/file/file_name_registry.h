#pragma once

#include <stddef.h>

typedef struct file_name_entry_s {
    char * absolute_path;
    char * include_path;

    struct file_name_entry_s * next;
    struct file_name_entry_s * prev;
} file_name_entry_t;

typedef struct {
    file_name_entry_t head;
    file_name_entry_t tail;
} file_name_registry_t;

void file_name_registry_init(file_name_registry_t * fnr);
void file_name_registry_free(file_name_registry_t * fnr);

file_name_entry_t * file_name_registry_push(file_name_registry_t * fnr, const char * absolute_path, const char * include_path);
void file_name_registry_remove(file_name_entry_t * file_name);