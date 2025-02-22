#pragma once

#include <stddef.h>

#include <line_buffer.h>
#include <preprocessor/preprocessor_token_buffer.h>
#include <preprocessor/variable_map.h>
#include <preprocessor/comment_filter.h>

#define PREPROCESSOR_DEFAULT_CAPACITY (256);

typedef struct {
    size_t current_depth;

    file_name_registry_t * fnr;
    file_name_entry_t * file_name;

    preprocessor_vmap_t vmap;
    preprocessor_token_buffer_t tbuf;
    preprocessor_comment_filter_t cfilter;
} preprocessor_t;

void preprocessor_init(preprocessor_t * preprocessor, file_name_registry_t * fnr, const char * absolute_path, const char * include_path, size_t depth);
void preprocessor_free(preprocessor_t * preprocessor);

void preprocessor_parse(preprocessor_t * preprocessor, const char * data);
void preprocessor_render(preprocessor_t * preprocessor, line_buffer_t * line_buffer);

void preprocessor_log_tokens(preprocessor_t * preprocessor);