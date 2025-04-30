#include <stdio.h>
#include <stdlib.h>

#include <alloc.h>
#include <options.h>
#include <parsing/is_symbol_char.h>
#include <debug/error_handler.h>
#include <info/version.h>
#include <info/usage.h>

options_t options = {
    .log_enable = false,
    .debug_enable = false,
    .color_enable = true,

    .phase1 = false,
    .phase2 = false,
    .phase3 = false,

    .preprocess_only = false,
    .dump_tree = false,

    .input_path = NULL,
    .output_path = NULL,

    .max_preprocessor_depth = 200,

    .max_integer_length = 48,
    .max_real_length = 48,
    .max_identifier_length = 48,
    .max_string_length = 1024,
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

                log_printf("Adding include directory \"%s\"\n", argv[i]);

                options.include_directories[options.include_directories_size++] = argv[i];

                if (options.include_directories_size == options.include_directories_capacity) {
                    options.include_directories_capacity *= 2;

                    options.include_directories = pkcc_realloc(options.include_directories, options.include_directories_capacity * sizeof(const char *));
                }
            }
            else if (strcmp(argv[i] + 1, "1") == 0) {
                options.phase1 = true;
            }
            else if (strcmp(argv[i] + 1, "2") == 0) {
                options.phase2 = true;
            }
            else if (strcmp(argv[i] + 1, "3") == 0) {
                options.phase3 = true;
            }
            else if (argv[i][1] == 'f') {
                const char * data = &argv[i][2];

                ssize_t equal_pos = 0;
                while (data[equal_pos] != '=') {
                    if (data[equal_pos] == '\0') {
                        equal_pos = -1;
                        break;
                    }
                    else if (!is_symbol_char(data[equal_pos]) && data[equal_pos] != '-') fatal_error("Invalid -f switch\nFormat -f<variable_name>[=<value>]\n");

                    equal_pos++;
                }

                if (equal_pos != -1) {
                    if (strncmp(data, "max-include-depth=", 18) == 0) {
                        char * end_ptr;
                        options.max_preprocessor_depth = strtoull(&data[equal_pos + 1], &end_ptr, 10);

                        if (!(data[equal_pos + 1] != '\0' && *end_ptr == '\0')) {
                            fatal_error("Invalid -f switch variable value\n");
                        }
                    }
                    else if (strncmp(data, "max-integer-literal-length=", 27) == 0) {
                        char * end_ptr;
                        options.max_integer_length = strtoull(&data[equal_pos + 1], &end_ptr, 10);

                        if (!(data[equal_pos + 1] != '\0' && *end_ptr == '\0')) {
                            fatal_error("Invalid -f switch variable value\n");
                        }
                    }
                    else if (strncmp(data, "max-real-constant-length=", 25) == 0) {
                        char * end_ptr;
                        options.max_real_length = strtoull(&data[equal_pos + 1], &end_ptr, 10);

                        if (!(data[equal_pos + 1] != '\0' && *end_ptr == '\0')) {
                            fatal_error("Invalid -f switch variable value\n");
                        }
                    }
                    else if (strncmp(data, "max-identifier-length=", 22) == 0) {
                        char * end_ptr;
                        options.max_identifier_length = strtoull(&data[equal_pos + 1], &end_ptr, 10);

                        if (!(data[equal_pos + 1] != '\0' && *end_ptr == '\0')) {
                            fatal_error("Invalid -f switch variable value\n");
                        }
                    }
                    else if (strncmp(data, "max-string-literal-length=", 26) == 0) {
                        char * end_ptr;
                        options.max_string_length = strtoull(&data[equal_pos + 1], &end_ptr, 10);

                        if (!(data[equal_pos + 1] != '\0' && *end_ptr == '\0')) {
                            fatal_error("Invalid -f switch variable value\n");
                        }
                    }
                    else {
                        fatal_error("Invalid -f switch\nInvalid variable name\n");
                    }
                }
                else {
                    if (strncmp(data, "no-diagnostics-color", 20) == 0) {
                        options.color_enable = false;
                    }
                    else if (strncmp(data, "dump-tree", 9) == 0) {
                        options.dump_tree = true;
                    }
                    else {
                        fatal_error("Invalid -f switch\nInvalid variable name\n");
                    }
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
                    "Multiple input files were provided ('%s' & '%s')\n",
                    options.input_path,
                    argv[i]
                );
            }

            options.input_path = argv[i];
        }
    }
}

void options_free(void) {
    pkcc_free(options.include_directories);
}