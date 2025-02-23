#pragma once

typedef struct free_list_node_s {
    void * object_pointer;
    void (* free_function)(void *);

    struct free_list_node_s * next;
    struct free_list_node_s * prev;
} free_list_node_t;

typedef struct {
    free_list_node_t * head;
    free_list_node_t tail;
} free_list_t;

void free_list_init(free_list_t * free_list);
void free_list_free(free_list_t * free_list);

free_list_node_t * free_list_push(free_list_t * free_list, void * object_pointer, void (* free_function)(void *));
void free_list_remove(free_list_node_t * node);