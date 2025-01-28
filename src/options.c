#include <options.h>

options_t options = {
    .log_enable = false,
};

bool options_parse_cli(int argc, const char ** argv) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i] + 1, "dd") == 0) {
                options.log_enable = true;
                options.debug_enable = true;
            }
            else if (strcmp(argv[i] + 1, "d") == 0) options.log_enable = true;
        }
    }
}