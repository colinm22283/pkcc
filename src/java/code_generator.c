#include <java/code_generator.h>
#include <java/header_footer.h>
#include <java/add_lib_definitions.h>
#include <java/generators/globals.h>
#include <java/generators/functions.h>

#include <alloc.h>

void java_code_generator_init(java_code_generator_t * jcg, FILE * out_file, syntax_tree_t * syntax_tree) {
    jcg->out_file = out_file;
    jcg->syntax_tree = syntax_tree;
    
    {
        size_t classname_start = strlen(options.output_path) - 1;
        while (options.output_path[classname_start] != '/' && classname_start != 0) classname_start--;
        if (options.output_path[classname_start] == '/') classname_start++;

        size_t classname_length = 0;
        while (options.output_path[classname_start + classname_length] != '.') classname_length++;

        jcg->classname = pkcc_alloc(classname_length + 1);
        memcpy(jcg->classname, &options.output_path[classname_start], classname_length);
        jcg->classname[classname_length] = '\0';
    }

    java_variable_registry_init(&jcg->jvr);

    if (java_getchar_type != NULL) java_variable_registry_push_function(&jcg->jvr, "getchar", java_getchar_type);
    if (java_putchar_type != NULL) java_variable_registry_push_function(&jcg->jvr, "putchar", java_putchar_type);
    if (java_getint_type != NULL) java_variable_registry_push_function(&jcg->jvr, "getint", java_getint_type);
    if (java_putint_type != NULL) java_variable_registry_push_function(&jcg->jvr, "putint", java_putint_type);
    if (java_getfloat_type != NULL) java_variable_registry_push_function(&jcg->jvr, "getfloat", java_getfloat_type);
    if (java_putfloat_type != NULL) java_variable_registry_push_function(&jcg->jvr, "putfloat", java_putfloat_type);
    if (java_putstring_type != NULL) java_variable_registry_push_function(&jcg->jvr, "putstring", java_putstring_type);
}

void java_code_generator_free(java_code_generator_t * jcg) {
    java_variable_registry_free(&jcg->jvr);

    pkcc_free(jcg->classname);
}

void java_code_generator_run(java_code_generator_t * jcg) {
    java_print_header(jcg->out_file, jcg->classname);

    java_generator_global(jcg);

    java_generator_function(jcg);

    java_print_footer(jcg->out_file, jcg->classname);
}