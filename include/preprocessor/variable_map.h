#pragma once

#define PREPROCESSOR_VMAP_BUCKETS (512)

typedef struct {
    size_t arg;
    size_t position;
} preprocessor_vmap_node_arg_t;

typedef struct preprocessor_vmap_node_s {
    char * key;
    char * value;

    struct preprocessor_vmap_node_s * next;
} preprocessor_vmap_node_t;

typedef struct {
    preprocessor_vmap_node_t * heads[PREPROCESSOR_VMAP_BUCKETS];
} preprocessor_vmap_t;

void preprocessor_vmap_init(preprocessor_vmap_t * vmap);
void preprocessor_vmap_free(preprocessor_vmap_t * vmap);

preprocessor_vmap_node_t * preprocessor_vmap_map(
    preprocessor_vmap_t * vmap,
    const char * restrict key,
    size_t key_len,
    const char * restrict value,
    size_t value_len
);

const char * preprocessor_vmap_get(preprocessor_vmap_t * vmap, const char * restrict key, size_t len);

bool preprocessor_vmap_unmap(preprocessor_vmap_t * vmap, const char * restrict key, size_t key_len);