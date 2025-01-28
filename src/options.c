#include <stdio.h>
#include <stdlib.h>

#include <options.h>
#include <debug/error_handler.h>
#include <info/version.h>
#include <info/usage.h>

options_t options = {
    .log_enable = false,
};

void options_parse_cli(int argc, const char ** argv) {
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
            else {
                fatal_error(
                    "Unknown command line argument '%s'\n"
                    USAGE_STRING,
                    argv[i],
                    argv[0]
                );
            }
        }
    }
}