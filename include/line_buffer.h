#pragma once

#include <stddef.h>

#include <file/file_name_registry.h>

typedef struct {
    file_name_entry_t * file_name;
    size_t source_line;
} line_buffer_metadata_t;

typedef struct {
    char * line;

    line_buffer_metadata_t metadata;
} line_buffer_line_t;

typedef struct {
    size_t size, capacity;
    line_buffer_line_t * lines;
} line_buffer_t;

void line_buffer_init(line_buffer_t * line_buffer);
void line_buffer_free(line_buffer_t * line_buffer);

void line_buffer_push_line(
    line_buffer_t * line_buffer,
    const char * line,
    size_t line_size,
    file_name_entry_t * file_name,
    size_t line_num
);