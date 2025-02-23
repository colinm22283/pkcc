#include <limits.h>

#include <alloc.h>
#include <file/file_loader.h>
#include <file/file_writer.h>

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

    file_name_registry_t fnr;
    file_name_registry_init(&fnr);

    char absolute_path_buffer[PATH_MAX];
    const char * absolute_path = realpath(options.input_path, absolute_path_buffer);

    preprocessor_t preprocessor;
    preprocessor_init(&preprocessor, &fnr, absolute_path, options.input_path, 0);

    scanner_t scanner;
    scanner_init(&scanner);

    line_buffer_t preprocessor_output;
    line_buffer_init(&preprocessor_output);

    preprocessor_parse(&preprocessor, file_loader.data);
    preprocessor_render(&preprocessor, &preprocessor_output);
    file_loader_free(&file_loader);

    if (options.preprocess_only) {
        file_writer_t file_writer;
        file_writer_init(&file_writer, options.output_path);

        file_writer_write_line_buffer(&file_writer, &preprocessor_output);

        file_writer_free(&file_writer);
        preprocessor_free(&preprocessor);
        line_buffer_free(&preprocessor_output);
        scanner_free(&scanner);

        file_name_registry_free(&fnr);

        return 0;
    }

    scanner_scan(&scanner, &preprocessor_output);

    line_buffer_free(&preprocessor_output);
    preprocessor_free(&preprocessor);

    if (options.log_enable) {
        log_printf("Printing scanner tokens:\n");
        scanner_print_tokens(&scanner);
    }

    scanner_free(&scanner);

    file_name_registry_free(&fnr);

    options_free();
}