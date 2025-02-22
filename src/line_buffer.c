#include <alloc.h>
#include <line_buffer.h>

#define LINE_BUFFER_INITIAL_SIZE (256)

void line_buffer_init(line_buffer_t * line_buffer) {
    line_buffer->size = 0;
    line_buffer->capacity = LINE_BUFFER_INITIAL_SIZE;
    line_buffer->lines = pkcc_alloc(line_buffer->capacity * sizeof(line_buffer_line_t));
}

void line_buffer_free(line_buffer_t * line_buffer) {
    for (size_t i = 0; i < line_buffer->size; i++) {
        line_buffer_line_t * line = &line_buffer->lines[i];

        pkcc_free(line->line);
    }

    pkcc_free(line_buffer->lines);
}

void line_buffer_push_line(
    line_buffer_t * line_buffer,
    const char * line_content,
    size_t line_size,
    file_name_entry_t * file_name,
    size_t _line
) {
    line_buffer_line_t * line = &line_buffer->lines[line_buffer->size++];

    if (line_buffer->size == line_buffer->capacity) {
        line_buffer->capacity *= 2;

        line_buffer->lines = pkcc_realloc(line_buffer->lines, line_buffer->capacity * sizeof(line_buffer_line_t));
    }

    line->line = pkcc_alloc(line_size + 1);
    memcpy(line->line, line_content, line_size);
    line->line[line_size] = '\0';

    line->metadata.file_name = file_name;
    line->metadata.source_line = _line;
}