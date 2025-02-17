#include <stdio.h>

#include <alloc.h>
#include <file_loader.h>
#include <debug/log.h>
#include <debug/error_handler.h>

void file_loader_init(file_loader_t * fl, const char * path) {
    log_printf("Initializing file reader on '%s'\n", path);

    FILE * file = fopen(path, "r");

    if (file == NULL) fatal_error("Unable to open file with path '%s'\n", path);

    fseek(file, 0, SEEK_END);
    ssize_t size = ftell(file);

    fseek(file, 0, SEEK_SET);

    fl->data = pkcc_alloc(size + 1);
    if (fread(fl->data, 1, size, file) != size) fatal_error("Unable to read file with path '%s'\n", path);

    fl->data[size] = '\0';

    fclose(file);
}

void file_loader_free(file_loader_t * fl) {
    log_printf("Freeing file reader\n");

    pkcc_free(fl->data);
}