#include <stdio.h>
#include <stdlib.h>

#include <alloc.h>
#include <options.h>
#include <debug/error_handler.h>
#include <info/version.h>
#include <info/usage.h>

options_t options = {
    .log_enable = false,
    .debug_enable = false,

    .preprocess_only = false,

    .input_path = NULL,
    .output_path = NULL,
};

void options_parse_cli(int argc, const char ** argv) {
    options.include_directories_size = 0;
    options.include_directories_capacity = 1;
    options.include_directories = pkcc_alloc(options.include_directories_capacity * sizeof(const char *));

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i] + 1, "dd") == 0) {
                options.log_enable = true;
                options.debug_enable = true;
            }
            else if (strcmp(argv[i] + 1, "d") == 0) options.log_enable = true;
            else if (
                strcmp(argv[i] + 1, "0") == 0 ||
                strcmp(argv[i] + 1, "v") == 0 ||
                strcmp(argv[i] + 1, "-version") == 0
            ) {
                printf(
                    "PKCC version %u.%u, compiled on " __DATE__ " " __TIME__ "\n"
                    "Author: Colin McGann (colinm22@iastate.edu)\n",
                    version_major,
                    version_minor
                );
                exit(0);
            }
            else if (
                strcmp(argv[i] + 1, "h") == 0 ||
                strcmp(argv[i] + 1, "-help") == 0
            ) {
                printf(USAGE_STRING, argv[0]);
                exit(0);
            }
            else if (strcmp(argv[i] + 1, "o") == 0) {
                i++;

                options.output_path = argv[i];
            }
            else if (strcmp(argv[i] + 1, "E") == 0) {
                options.preprocess_only = true;
            }
            else if (strcmp(argv[i] + 1, "I") == 0) {
                i++;

                options.include_directories[options.include_directories_size++] = argv[i];

                if (options.include_directories_size == options.include_directories_capacity) {
                    options.include_directories_capacity *= 2;

                    options.include_directories = pkcc_realloc(options.include_directories, options.include_directories_capacity * sizeof(const char *));
                }
            }
            else {
                fatal_error(
                    "Unknown command line argument '%s'\n"
                    USAGE_STRING,
                    argv[i],
                    argv[0]
                );
            }
        }
        else {
            if (options.input_path != NULL) {
                fatal_error(
                    "Multiple input files were provided (eg. '%s' & '%s')\n",
                    options.input_path,
                    argv[i]
                );
            }

            options.input_path = argv[i];
        }
    }
}