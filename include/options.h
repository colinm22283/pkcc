#pragma once

#include <stdbool.h>
#include <string.h>

typedef struct {
    bool log_enable;
    bool debug_enable;
    bool color_enable;

    bool preprocess_only;

    const char * input_path;
    const char * output_path;

    size_t include_directories_size, include_directories_capacity;
    const char ** include_directories;

    size_t max_preprocessor_depth;
} options_t;

extern options_t options;

void options_parse_cli(int argc, const char ** argv);
void options_free(void);