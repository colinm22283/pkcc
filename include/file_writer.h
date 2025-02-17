#pragma once

#include <stdio.h>

typedef struct {
    FILE * file;
} file_writer_t;

void file_writer_init(file_writer_t * fw, const char * path);
void file_writer_free(file_writer_t * fw);

void file_writer_write_string(file_writer_t * fw, const char * str);
void file_writer_write_array(file_writer_t * fw, const char * arr, size_t size);