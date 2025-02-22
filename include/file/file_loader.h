#pragma once

typedef struct {
    char * data;
} file_loader_t;

void file_loader_init(file_loader_t * fl, const char * path);
void file_loader_free(file_loader_t * fl);