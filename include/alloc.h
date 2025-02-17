#pragma once

#include <stdlib.h>

#include <debug/error_handler.h>
#include <debug/log.h>

static inline void * pkcc_alloc(size_t size) {
    debug_printf("Allocating %zu bytes at ", size);

    void * alloc = malloc(size);

    if (alloc == NULL) fatal_error("Malloc failed to allocate %zu bytes", size);

    debug_printf("0x%zx\n", alloc);

    return alloc;
}

static inline void * pkcc_realloc(void * old, size_t size) {
    debug_printf("Reallocating %zu bytes from 0x%zx to ", size, old);

    void * alloc = realloc(old, size);

    if (alloc == NULL) fatal_error("Realloc failed to allocate %zu bytes", size);

    debug_printf("0x%zx\n", alloc);

    return alloc;
}

static inline void pkcc_free(void * alloc) {
    debug_printf("Freeing memory at 0x%zx\n", alloc);

    free(alloc);
}