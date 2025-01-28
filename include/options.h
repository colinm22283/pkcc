#pragma once

#include <stdbool.h>
#include <string.h>

typedef struct {
    bool log_enable;
    bool debug_enable;
} options_t;

extern options_t options;

void options_parse_cli(int argc, const char ** argv);