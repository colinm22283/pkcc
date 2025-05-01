#include <stdio.h>

#include <main.h>
#include <options.h>
#include <debug/line_error.h>
#include <debug/output_colors.h>

__NORETURN void fatal_line_range_error(line_buffer_t * line_buffer, file_name_entry_t * file_name, const char * message, size_t line, size_t position, size_t size) {
    if (options.color_enable) fprintf(stderr, OUTPUT_COLOR_ERROR "ERROR:\n");
    else fprintf(stderr, "ERROR:\n");

    fprintf(stderr, "%s: %s on line %zu at position %zu\n", file_name->absolute_path, message, line_buffer->lines[line].metadata.source_line + 1, position + 1);

    if (line != 0) fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line - 1].metadata.source_line + 1, line_buffer->lines[line - 1].line);
    if (options.color_enable) {
        fprintf(stderr, "%5zu: ", line_buffer->lines[line].metadata.source_line + 1);
        size_t i;
        for (i = 0; i < position; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fprintf(stderr, OUTPUT_COLOR_ERROR_HIGHLIGHT);
        for (; i < position + size; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fprintf(stderr, OUTPUT_COLOR_ERROR);
        for (; line_buffer->lines[line].line[i] != '\0'; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fputc('\n', stderr);
    }
    else {
        fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line].metadata.source_line + 1, line_buffer->lines[line].line);
        fprintf(stderr, "%*c", (int) position + 7, ' ');
        for (size_t i = 0; i < size; i++) fputc('^', stderr);
        fputc('\n', stderr);
    }
    if (line != line_buffer->size - 1) fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line + 1].metadata.source_line + 1, line_buffer->lines[line + 1].line);

    if (options.color_enable) fprintf(stderr, OUTPUT_COLOR_RESET);

    exit_and_free(1);
}

__NORETURN void fatal_formatted_line_range_error_v(line_buffer_t * line_buffer, file_name_entry_t * file_name, size_t line, size_t position, size_t size, const char * message, va_list args) {
    if (options.color_enable) fprintf(stderr, OUTPUT_COLOR_ERROR "ERROR:\n");
    else fprintf(stderr, "ERROR:\n");

    fprintf(stderr, "%s: ", file_name->absolute_path);

    vfprintf(stderr, message, args);

    fprintf(stderr, " on line %zu at position %zu\n", line_buffer->lines[line].metadata.source_line + 1, position + 1);

    if (line != 0) fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line - 1].metadata.source_line + 1, line_buffer->lines[line - 1].line);
    if (options.color_enable) {
        fprintf(stderr, "%5zu: ", line_buffer->lines[line].metadata.source_line + 1);
        size_t i;
        for (i = 0; i < position; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fprintf(stderr, OUTPUT_COLOR_ERROR_HIGHLIGHT);
        for (; i < position + size; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fprintf(stderr, OUTPUT_COLOR_ERROR);
        for (; line_buffer->lines[line].line[i] != '\0'; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fputc('\n', stderr);
    }
    else {
        fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line].metadata.source_line + 1, line_buffer->lines[line].line);
        fprintf(stderr, "%*c", (int) position + 7, ' ');
        for (size_t i = 0; i < size; i++) fputc('^', stderr);
        fputc('\n', stderr);
    }
    if (line != line_buffer->size - 1) fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line + 1].metadata.source_line + 1, line_buffer->lines[line + 1].line);

    if (options.color_enable) fprintf(stderr, OUTPUT_COLOR_RESET);

    exit_and_free(1);
}