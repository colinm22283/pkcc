#include <stddef.h>

#include <alloc.h>

#include <preprocessor/variable_map.h>

void preprocessor_vmap_init(preprocessor_vmap_t * vmap) {
    for (size_t i = 0; i < PREPROCESSOR_VMAP_BUCKETS; i++) vmap->heads[i] = NULL;
}

void preprocessor_vmap_free(preprocessor_vmap_t * vmap) {
    for (size_t i = 0; i < PREPROCESSOR_VMAP_BUCKETS; i++) {
        preprocessor_vmap_node_t * node = vmap->heads[i];

        while (node != NULL) {
            preprocessor_vmap_node_t * old_node = node;
            node = node->next;

            pkcc_free(old_node->key);
            pkcc_free(old_node->value);
            pkcc_free(old_node);
        }
    }
}

static inline size_t hash_func(const char * key, size_t size) {
    size_t hash = 0;
    for (size_t i = 0; i < size; i++) hash += key[i];

    return hash % PREPROCESSOR_VMAP_BUCKETS;
}

preprocessor_vmap_node_t * preprocessor_vmap_map(
    preprocessor_vmap_t * vmap,
    const char * restrict key,
    size_t key_len,
    const char * restrict value,
    size_t value_len
) {
    size_t hash = hash_func(key, key_len);

    preprocessor_vmap_node_t * new_node = pkcc_alloc(sizeof(preprocessor_vmap_node_t));

    new_node->key = pkcc_alloc(key_len + 1);
    memcpy(new_node->key, key, key_len);
    new_node->key[key_len] = '\0';

    new_node->value = pkcc_alloc(value_len + 1);
    memcpy(new_node->value, value, value_len);
    new_node->value[value_len] = '\0';

    new_node->next = vmap->heads[hash];
    vmap->heads[hash] = new_node;

    return new_node;
}

const char * preprocessor_vmap_get(preprocessor_vmap_t * vmap, const char * restrict key, size_t len) {
    if (len == 0) return NULL;

    size_t hash = hash_func(key, len);

    preprocessor_vmap_node_t * node = vmap->heads[hash];

    while (node != NULL) {
        if (strncmp(node->key, key, len) == 0) return node->value;

        node = node->next;
    }

    return NULL;
}

bool preprocessor_vmap_unmap(preprocessor_vmap_t * vmap, const char * restrict key, size_t key_len) {
    size_t hash = hash_func(key, key_len);

    preprocessor_vmap_node_t ** node = &vmap->heads[hash];

    while (*node != NULL) {
        if (strncmp((*node)->key, key, key_len) == 0) {
            preprocessor_vmap_node_t * old_node = *node;

            *node = old_node->next;

            pkcc_free(old_node->key);
            pkcc_free(old_node->value);
            pkcc_free(old_node);

            return true;
        }

        node = &(*node)->next;
    }

    return false;
}