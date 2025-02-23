#include <stddef.h>

#include <alloc.h>
#include <free_list.h>
#include <debug/log.h>

void free_list_init(free_list_t * free_list) {
    log_printf("Initializing free list\n");

    free_list->head = &free_list->tail;

    free_list->head->prev = NULL;
    free_list->head->next = NULL;
}

void free_list_free(free_list_t * free_list) {
    log_printf("Freeing free list\n");

    free_list_node_t * node = free_list->head;

    while (node != &free_list->tail) {
        free_list_node_t * next = node->next;

        node->free_function(node->object_pointer);
        pkcc_free(node);

        node = next;
    }
}

free_list_node_t * free_list_push(free_list_t * free_list, void * object_pointer, void (* free_function)(void *)) {
    free_list_node_t * new_node = pkcc_alloc(sizeof(free_list_node_t));

    new_node->next = free_list->head;
    new_node->prev = NULL;
    free_list->head->prev = new_node;
    free_list->head = new_node;

    new_node->object_pointer = object_pointer;
    new_node->free_function = free_function;

    return new_node;
}

void free_list_remove(free_list_node_t * node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;

    node->free_function(node->object_pointer);

    pkcc_free(node);
}