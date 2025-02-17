#pragma once

#include <stddef.h>

#include <preprocessor/preprocessor_token_buffer.h>
#include <preprocessor/variable_map.h>
#include <preprocessor/comment_filter.h>

#define PREPROCESSOR_DEFAULT_CAPACITY (256);

typedef struct {
    preprocessor_vmap_t vmap;
    preprocessor_token_buffer_t tbuf;
    preprocessor_comment_filter_t cfilter;

    size_t output_size, output_capacity;
    char * output;
} preprocessor_t;

void preprocessor_init(preprocessor_t * preprocessor);
void preprocessor_free(preprocessor_t * preprocessor);

void preprocessor_parse(preprocessor_t * preprocessor, const char * data);
void preprocessor_render(preprocessor_t * preprocessor);