#pragma once

#include <defs.h>
#include <line_buffer.h>
#include <file/file_name_registry.h>

void fatal_line_range_warning(line_buffer_t * line_buffer, file_name_entry_t * file_name, const char * message, size_t line, size_t position, size_t size);
static inline void fatal_line_warning(line_buffer_t * line_buffer, file_name_entry_t * file_name, const char * message, size_t line, size_t position) {
    fatal_line_range_warning(line_buffer, file_name, message, line, position, 1);
}