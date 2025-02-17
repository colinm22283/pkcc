#pragma once

#define USAGE_STRING \
    "Usage: %s [options] source_file\n" \
    "Options:\n" \
    "  -d                     Enable partial debug messages\n"              \
    "  -dd                    Enable full debug messages\n"                 \
    "  -0, -v,   version      Print version information\n"                  \
    "  -h, --help             Print usage information\n"                      \
    "  -E                     Output only preprocessed input to file\n"       \
    "  -I                     Add a directory to the list of include paths\n" \
    "  -o <out_path>          Output to path <out_path>\n"
