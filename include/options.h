#pragma once

#include <stdbool.h>
#include <string.h>

typedef struct {
    bool log_enable;
    bool debug_enable;
    bool color_enable;

    bool phase1;
    bool phase2;
    bool phase3;
    bool phase4;
    bool phase5;

    bool preprocess_only;
    bool dump_tree;

    const char * input_path;
    const char * output_path;

    size_t include_directories_size, include_directories_capacity;
    const char ** include_directories;

    size_t max_preprocessor_depth;

    size_t max_integer_length;
    size_t max_real_length;
    size_t max_identifier_length;
    size_t max_string_length;

    const char * backend_implementation;
} options_t;

extern options_t options;

void options_parse_cli(int argc, const char ** argv);
void options_free(void);