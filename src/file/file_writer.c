#include <string.h>

#include <file/file_writer.h>

void file_writer_init(file_writer_t * fw, const char * path) {
    fw->file = fopen(path, "w");
}

void file_writer_free(file_writer_t * fw) {
    fclose(fw->file);
}

void file_writer_write_string(file_writer_t * fw, const char * str) {
    fputs(str, fw->file);
}

void file_writer_write_array(file_writer_t * fw, const char * arr, size_t size) {
    fwrite(arr, 1, size, fw->file);
}

void file_writer_write_line_buffer(file_writer_t * fw, line_buffer_t * lb) {
    char newline = '\n';

    size_t line = 0;
    for (size_t i = 0; i < lb->size; i++) {
        if (line != lb->lines[i].metadata.source_line) {
            line = lb->lines[i].metadata.source_line;
            fwrite(&newline, 1, 1, fw->file);
        }
        
        fwrite(lb->lines[i].line, 1, strlen(lb->lines[i].line), fw->file);
    }

    fwrite(&newline, 1, 1, fw->file);
}