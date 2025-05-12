#include <java/code_generator.h>
#include <java/header_footer.h>
#include <java/generators/globals.h>
#include <java/generators/functions.h>

#include <alloc.h>

void java_code_generator_init(java_code_generator_t * jcg, FILE * out_file, syntax_tree_t * syntax_tree) {
    jcg->out_file = out_file;
    jcg->syntax_tree = syntax_tree;

    {
        size_t classname_start = strlen(options.output_path) - 1;
        while (options.output_path[classname_start] != '/') classname_start--;
        classname_start++;

        size_t classname_length = 0;
        while (options.output_path[classname_start + classname_length] != '.') classname_length++;

        jcg->classname = pkcc_alloc(classname_length + 1);
        memcpy(jcg->classname, &options.output_path[classname_start], classname_length);
        jcg->classname[classname_length] = '\0';
    }

    java_variable_registry_init(&jcg->jvr);
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