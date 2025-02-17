#include <alloc.h>
#include <file_loader.h>
#include <file_writer.h>

#include <debug/log.h>

#include <info/usage.h>

#include <preprocessor/preprocessor.h>

#include <scanner/scanner.h>

int main(int argc, const char ** argv) {
    options_parse_cli(argc, argv);

    log_printf("PKCC Compiler\n");

    if (options.input_path == NULL) fatal_error("No input path specified\n" USAGE_STRING, argv[0]);
    if (options.output_path == NULL) fatal_error("No output path specified\n" USAGE_STRING, argv[0]);

    file_loader_t file_loader;
    file_loader_init(&file_loader, options.input_path);

    preprocessor_t preprocessor;
    preprocessor_init(&preprocessor);

    scanner_t scanner;
    scanner_init(&scanner);

    preprocessor_parse(&preprocessor, file_loader.data);
    preprocessor_render(&preprocessor);
    file_loader_free(&file_loader);

    if (options.preprocess_only) {
        file_writer_t file_writer;
        file_writer_init(&file_writer, options.output_path);

        file_writer_write_string(&file_writer, preprocessor.output);

        file_writer_free(&file_writer);
        preprocessor_free(&preprocessor);
        scanner_free(&scanner);

        return 0;
    }

    scanner_scan(&scanner, preprocessor.output);
    preprocessor_free(&preprocessor);

    if (options.log_enable) {
        log_printf("Printing scanner tokens:\n");
        scanner_print_tokens(&scanner);
    }

    scanner_free(&scanner);
}