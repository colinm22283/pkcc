#include <alloc.h>
#include <file/file_name_registry.h>

void file_name_registry_init(file_name_registry_t * fnr) {
    log_printf("Initializing file name registry\n");

    fnr->head.next = &fnr->tail;
    fnr->head.prev = NULL;

    fnr->tail.next = NULL;
    fnr->tail.prev = &fnr->head;
}

void file_name_registry_free(file_name_registry_t * fnr) {
    log_printf("Freeing file name registry\n");

    file_name_entry_t * entry = fnr->head.next;

    while (entry != &fnr->tail) {
        file_name_entry_t * next = entry->next;

        pkcc_free(entry->absolute_path);
        pkcc_free(entry->include_path);
        pkcc_free(entry);

        entry = next;
    }
}

file_name_entry_t * file_name_registry_push(file_name_registry_t * fnr, const char * absolute_path, const char * include_path) {
    log_printf("Pushing entry to file name buffer\n\tAbsolute: %s\n\tInclude: %s\n", absolute_path, include_path);

    file_name_entry_t * new_entry = pkcc_alloc(sizeof(file_name_entry_t));

    new_entry->next = fnr->head.next;
    new_entry->prev = &fnr->head;
    fnr->head.next->prev = new_entry;
    fnr->head.next = new_entry;

    new_entry->absolute_path = pkcc_alloc(strlen(absolute_path) + 1);
    strcpy(new_entry->absolute_path, absolute_path);
    new_entry->include_path = pkcc_alloc(strlen(include_path) + 1);
    strcpy(new_entry->include_path, include_path);

    return new_entry;
}

void file_name_registry_remove(file_name_entry_t * file_name) {
    file_name->prev->next = file_name->next;
    file_name->next->prev = file_name->prev;

    pkcc_free(file_name->absolute_path);
    pkcc_free(file_name->include_path);
    pkcc_free(file_name);
}