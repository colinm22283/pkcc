#include <file_writer.h>

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