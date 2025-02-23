#include <stdio.h>
#include <stdlib.h>

#include <main.h>
#include <options.h>
#include <debug/line_error.h>

__NORETURN void fatal_line_range_error(line_buffer_t * line_buffer, file_name_entry_t * file_name, const char * message, size_t line, size_t position, size_t size) {
    if (options.color_enable) fprintf(stderr, "\033[31;1mERROR:\n");
    else fprintf(stderr, "ERROR:\n");

    fprintf(stderr, "%s: %s on line %zu at position %zu\n\n", file_name->absolute_path, message, line + 1, position + 1);

    if (line != 0) fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line - 1].metadata.source_line + 1, line_buffer->lines[line - 1].line);
    if (options.color_enable) {
        fprintf(stderr, "%5zu: \n", line_buffer->lines[line].metadata.source_line + 1);
        size_t i;
        for (i = 0; i < position; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fprintf(stderr, "\033[41;30;1m");
        for (; i < position + size; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fprintf(stderr, "\033[0;31;1m");
        for (; line_buffer->lines[line].line[i] != '\0'; i++) fputc(line_buffer->lines[line].line[i], stderr);
        fputc('\n', stderr);
    }
    else {
        fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line].metadata.source_line + 1, line_buffer->lines[line].line);
        fprintf(stderr, "%*c", (int) position, ' ');
        for (size_t i = 0; i < size; i++) fputc('^', stderr);
    }
    if (line != line_buffer->size - 1) fprintf(stderr, "%5zu: %s\n", line_buffer->lines[line + 1].metadata.source_line + 1, line_buffer->lines[line + 1].line);

    if (options.color_enable) fprintf(stderr, "\033[0m");

    exit_and_free(1);
}