#include <stdio.h>

#include <options.h>
#include <debug/line_warning.h>
#include <debug/output_colors.h>

void fatal_line_range_warning(line_buffer_t * line_buffer, file_name_entry_t * file_name, const char * message, size_t line, size_t position, size_t size) {
    if (options.color_enable) fprintf(stderr, OUTPUT_COLOR_WARNING "WARNING:\n");
    else fprintf(stderr, "ERROR:\n");

    fprintf(stderr, "%s: %s on line %zu at position %zu\n\n", file_name->absolute_path, message, line + 1, position + 1);

    if (line != 0) fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line - 1].metadata.source_line + 1, line_buffer->lines[line - 1].line);
    if (options.color_enable) {
        fprintf(stderr, "%5zu: \n", line_buffer->lines[line].metadata.source_line + 1);
        size_t i;
        for (i = 0; i < position; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fprintf(stderr, OUTPUT_COLOR_WARNING_HIGHLIGHT);
        for (; i < position + size; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fprintf(stderr, OUTPUT_COLOR_WARNING);
        for (; line_buffer->lines[line].line[i] != '\0'; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fputc('\n', stderr);
    }
    else {
        fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line].metadata.source_line + 1, line_buffer->lines[line].line);
        fprintf(stderr, "%*c", (int) position, ' ');
        for (size_t i = 0; i < size; i++) fputc('^', stderr);
    }
    if (line != line_buffer->size - 1) fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line + 1].metadata.source_line + 1, line_buffer->lines[line + 1].line);

    if (options.color_enable) fprintf(stderr, OUTPUT_COLOR_RESET);
}