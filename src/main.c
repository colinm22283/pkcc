#include <alloc.h>
#include <info/usage.h>
#include <debug/log.h>
#include <scanner/scanner.h>
#include <scanner/file_loader.h>

int main(int argc, const char ** argv) {
    log_printf("PKCC Compiler\n");

    options_parse_cli(argc, argv);

    if (options.input_path == NULL) fatal_error("No input path specified\n" USAGE_STRING, argv[0]);
    if (options.output_path == NULL) fatal_error("No output path specified\n" USAGE_STRING, argv[0]);

    file_loader_t file_loader;
    file_loader_init(&file_loader, options.input_path);

    scanner_t scanner;
    scanner_init(&scanner);

    scanner_scan(&scanner, file_loader.data);
    file_loader_free(&file_loader);

    if (options.debug_enable) {
        debug_printf("Printing scanner tokens:\n");
        scanner_print_tokens(&scanner);
    }

    scanner_free(&scanner);
}