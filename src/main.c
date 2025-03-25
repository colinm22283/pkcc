#include <limits.h>

#include <defs.h>
#include <main.h>
#include <alloc.h>
#include <free_list.h>
#include <file/file_loader.h>
#include <file/file_writer.h>

#include <debug/log.h>

#include <info/usage.h>

#include <preprocessor/preprocessor.h>

#include <scanner/scanner.h>
#include <scanner/phase1.h>

#include <lexer/lexer.h>
#include <lexer/rule_registry.h>

free_list_t free_list;

int main(int argc, const char ** argv) {
    log_printf("PKCC Compiler\n");

    free_list_init(&free_list);

    options_parse_cli(argc, argv);

    rule_registry_init();

    if (options.input_path == NULL) fatal_error("No input path specified\n" USAGE_STRING, argv[0]);
    if (options.output_path == NULL) fatal_error("No output path specified\n" USAGE_STRING, argv[0]);

    file_loader_t file_loader;
    file_loader_init(&file_loader, options.input_path);
    free_list_node_t * file_loader_node = free_list_push(&free_list, &file_loader, (void (*)(void *)) file_loader_free);

    file_name_registry_t fnr;
    file_name_registry_init(&fnr);
    __MAYBE_UNUSED free_list_node_t * fnr_node = free_list_push(&free_list, &fnr, (void (*)(void *)) file_name_registry_free);

    char absolute_path_buffer[PATH_MAX];
    const char * absolute_path = realpath(options.input_path, absolute_path_buffer);

    preprocessor_t preprocessor;
    preprocessor_init(&preprocessor, &fnr, absolute_path, options.input_path, 0);
    __MAYBE_UNUSED free_list_node_t * preprocessor_node = free_list_push(&free_list, &preprocessor, (void (*)(void *)) preprocessor_free);

    line_buffer_t preprocessor_output;
    line_buffer_init(&preprocessor_output);
    __MAYBE_UNUSED free_list_node_t * preprocessor_output_node = free_list_push(&free_list, &preprocessor_output, (void (*)(void *)) line_buffer_free);

    preprocessor_parse(&preprocessor, file_loader.data);
    preprocessor_render(&preprocessor, &preprocessor_output);
    free_list_remove(file_loader_node);

    if (options.preprocess_only) {
        file_writer_t file_writer;
        file_writer_init(&file_writer, options.output_path);
        __MAYBE_UNUSED free_list_node_t * file_writer_node = free_list_push(&free_list, &file_writer, (void (*)(void *)) file_writer_free);

        file_writer_write_line_buffer(&file_writer, &preprocessor_output);

        exit_and_free(0);
    }

    scanner_t scanner;
    scanner_init(&scanner);
    __MAYBE_UNUSED free_list_node_t * scanner_node = free_list_push(&free_list, &scanner, (void (*)(void *)) scanner_free);

    scanner_scan(&scanner, &preprocessor_output);

    if (options.log_enable) {
        log_printf("Printing scanner tokens:\n");
        scanner_print_tokens(&scanner);
    }

    if (options.phase1) {
        scanner_print_phase1(stdout, &scanner);

        FILE * out_file = fopen(options.output_path, "w");
        scanner_print_phase1(out_file, &scanner);
        fclose(out_file);

        exit_and_free(0);
    }

    lexer_t lexer;
    lexer_init(&lexer, &scanner);
    __MAYBE_UNUSED free_list_node_t * lexer_node = free_list_push(&free_list, &lexer, (void (*)(void *)) lexer_free);

    lexer_run(&lexer);

    exit_and_free(0);
}

__NORETURN void exit_and_free(int exit_code) {
    free_list_free(&free_list);

    options_free();

    exit(exit_code);
}