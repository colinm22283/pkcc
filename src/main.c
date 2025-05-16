#include <limits.h>

#include <defs.h>
#include <main.h>
#include <alloc.h>
#include <file/file_loader.h>
#include <file/file_writer.h>

#include <debug/log.h>

#include <info/usage.h>

#include <preprocessor/preprocessor.h>

#include <scanner/scanner.h>
#include <scanner/phase1.h>

#include <parser/parser.h>
#include <parser/rule_registry.h>
#include <parser/phase2.h>
#include <parser/type_checker/phase3.h>

#include <java/code_generator.h>
#include <java/add_lib_definitions.h>

free_list_t free_list;

int main(int argc, const char ** argv) {
    log_printf("PKCC Compiler\n");

    free_list_init(&free_list);

    options_parse_cli(argc, argv);

    rule_registry_init();

    if (options.input_path == NULL) fatal_error("No input path specified\n" USAGE_STRING, argv[0]);
    if (options.output_path == NULL) fatal_error("No output path specified\n" USAGE_STRING, argv[0]);

    static file_loader_t file_loader;
    file_loader_init(&file_loader, options.input_path);
    free_list_node_t * file_loader_node = free_list_push(&free_list, &file_loader, (void (*)(void *)) file_loader_free);

    static file_name_registry_t fnr;
    file_name_registry_init(&fnr);
    __MAYBE_UNUSED free_list_node_t * fnr_node = free_list_push(&free_list, &fnr, (void (*)(void *)) file_name_registry_free);

    char absolute_path_buffer[PATH_MAX];
    const char * absolute_path = realpath(options.input_path, absolute_path_buffer);

    static preprocessor_t preprocessor;
    preprocessor_init(&preprocessor, &fnr, absolute_path, options.input_path, 0);
    __MAYBE_UNUSED free_list_node_t * preprocessor_node = free_list_push(&free_list, &preprocessor, (void (*)(void *)) preprocessor_free);

    static line_buffer_t preprocessor_output;
    line_buffer_init(&preprocessor_output);
    __MAYBE_UNUSED free_list_node_t * preprocessor_output_node = free_list_push(&free_list, &preprocessor_output, (void (*)(void *)) line_buffer_free);

    preprocessor_parse(&preprocessor, file_loader.data);
    preprocessor_render(&preprocessor, &preprocessor_output);
    free_list_remove(file_loader_node);

    if (options.preprocess_only) {
        static file_writer_t file_writer;
        file_writer_init(&file_writer, options.output_path);
        __MAYBE_UNUSED free_list_node_t * file_writer_node = free_list_push(&free_list, &file_writer, (void (*)(void *)) file_writer_free);

        file_writer_write_line_buffer(&file_writer, &preprocessor_output);

        exit_and_free(0);
    }

    static scanner_t scanner;
    scanner_init(&scanner);
    __MAYBE_UNUSED free_list_node_t * scanner_node = free_list_push(&free_list, &scanner, (void (*)(void *)) scanner_free);

    scanner_scan(&scanner, &preprocessor_output);

    if (options.log_enable) {
        log_printf("Printing scanner tokens:\n");
        scanner_print_tokens(&scanner, &preprocessor_output);
    }

    if (options.phase1) {
        scanner_print_phase1(stdout, &scanner);

        FILE * out_file = fopen(options.output_path, "w");
        scanner_print_phase1(out_file, &scanner);
        fclose(out_file);

        exit_and_free(0);
    }

    static parser_t parser;
    parser_init(&parser, &scanner, &preprocessor_output);
    __MAYBE_UNUSED free_list_node_t * parser_node = free_list_push(&free_list, &parser, (void (*)(void *)) parser_free);

    if (options.phase4) {
        java_add_lib_definitions(&parser.type_checker.type_registry, &parser.type_checker.variable_registry);
    }

    parser_run(&parser);

    if (options.dump_tree) syntax_tree_print(&parser.syntax_tree);

    if (options.phase2) {
        parser_print_phase2(stdout, &parser);

        FILE * out_file = fopen(options.output_path, "w");
        parser_print_phase2(out_file, &parser);
        fclose(out_file);

        exit_and_free(0);
    }

    if (options.phase3) {
        type_verifier_phase3(
            stdout,
            parser.type_checker.line_buffer,
            parser.type_checker.token_buffer,
            parser.type_checker.syntax_tree
        );

        FILE * out_file = fopen(options.output_path, "w");
        type_verifier_phase3(
            out_file,
            parser.type_checker.line_buffer,
            parser.type_checker.token_buffer,
            parser.type_checker.syntax_tree
        );
        fclose(out_file);

        exit_and_free(0);
    }

    if (options.phase4) {
        FILE * out_file = fopen(options.output_path, "w");

        java_code_generator_t jcg;
        java_code_generator_init(&jcg, out_file, &parser.syntax_tree);
        __MAYBE_UNUSED free_list_node_t * jcg_node = free_list_push(&free_list, &jcg, (void (*)(void *)) java_code_generator_free);

        java_code_generator_run(&jcg);

        fclose(out_file);

        exit_and_free(0);
    }

    exit_and_free(0);
}

__NORETURN void exit_and_free(int exit_code) {
    free_list_free(&free_list);

    options_free();

    exit(exit_code);
}