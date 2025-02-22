#pragma once

#include <stddef.h>

typedef struct {
    size_t data_size;
    size_t data_capacity;
    char * data;
} preprocessor_comment_filter_t;

void preprocessor_comment_filter_init(preprocessor_comment_filter_t * cf);
void preprocessor_comment_filter_free(preprocessor_comment_filter_t * cf);

void preprocessor_comment_filter_parse(preprocessor_comment_filter_t * cf, const char * input);
