#pragma once

#include <string.h>
#include <stdarg.h>

#include <defs.h>
#include <line_buffer.h>
#include <file/file_name_registry.h>

__NORETURN void fatal_line_range_error(line_buffer_t * line_buffer, file_name_entry_t * file_name, const char * message, size_t line, size_t position, size_t size);
__NORETURN static inline void fatal_line_error(line_buffer_t * line_buffer, file_name_entry_t * file_name, const char * message, size_t line, size_t position) {
    fatal_line_range_error(line_buffer, file_name, message, line, position, 1);
}
__NORETURN static inline void fatal_line_full_error(line_buffer_t * line_buffer, file_name_entry_t * file_name, const char * message, size_t line) {
    fatal_line_range_error(line_buffer, file_name, message, line, 0, strlen(line_buffer->lines[line].line));
}

__NORETURN void fatal_formatted_line_range_error_v(line_buffer_t * line_buffer, file_name_entry_t * file_name, size_t line, size_t position, size_t size, const char * message, va_list args);

__NORETURN static inline void fatal_formatted_line_range_error(line_buffer_t * line_buffer, file_name_entry_t * file_name, size_t line, size_t position, size_t size, const char * message, ...) {
    va_list args;
    va_start(args, message);

    fatal_formatted_line_range_error_v(line_buffer, file_name, line, position, size, message, args);

    va_end(args);
}
__NORETURN static inline void fatal_formatted_line_error(line_buffer_t * line_buffer, file_name_entry_t * file_name, size_t line, size_t position, const char * message, ...) {
    va_list args;
    va_start(args, message);

    fatal_formatted_line_range_error_v(line_buffer, file_name, line, position, 1, message, args);

    va_end(args);
}
__NORETURN static inline void fatal_formatted_line_full_error(line_buffer_t * line_buffer, file_name_entry_t * file_name, size_t line, const char * message, ...) {
    va_list args;
    va_start(args, message);

    fatal_formatted_line_range_error_v(line_buffer, file_name, line, 0, strlen(line_buffer->lines[line].line), message, args);

    va_end(args);
}