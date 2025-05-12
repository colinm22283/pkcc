#include <debug/log.h>

#include <parser/syntax_tree_traverser.h>
#include <parser/rules.h>

#include <java/java_type_stringify.h>
#include <java/generators/globals.h>
#include <java/generators/expression.h>

typedef struct {
    const char * name;
    type_checker_type_t * type;
    type_checker_scope_t * scope;
    syntax_tree_traverser_t expression_stt;
} java_generator_global_t;

void java_generator_global(java_code_generator_t * jcg) {
    size_t global_count = 0, global_capacity = 1;
    java_generator_global_t * globals = pkcc_alloc(global_capacity * sizeof(java_generator_global_t));

    syntax_tree_traverser_t start_stt;
    syntax_tree_traverser_init(&start_stt, &jcg->syntax_tree->head);

    while (syntax_tree_traverser_seek_nonterminal(&start_stt, NT_DECL)) {
        syntax_tree_traverser_t decl_stt;
        syntax_tree_traverser_enter(&start_stt, &decl_stt);

        if (syntax_tree_traverser_seek_nonterminal(&decl_stt, NT_DECL_VAR)) {
            log_printf("Got variable declaration\n");

            syntax_tree_traverser_t decl_var_stt;
            syntax_tree_traverser_enter(&decl_stt, &decl_var_stt);

            syntax_tree_traverser_seek_nonterminal(&decl_var_stt, NT_TYPE);
            type_checker_type_t * decl_type = decl_var_stt.current_node->type;
            type_checker_scope_t * decl_scope = decl_var_stt.current_node->scope;

            if (syntax_tree_traverser_seek_nonterminal(&decl_var_stt, NT_DECL_VAR_NAME_LIST)) {
                syntax_tree_traverser_t decl_list_stt;
                syntax_tree_traverser_enter(&decl_var_stt, &decl_list_stt);

                while (true) {
                    syntax_tree_traverser_seek_nonterminal(&decl_list_stt, NT_DECL_VAR_DECL);

                    syntax_tree_traverser_t decl_var_decl_stt;
                    syntax_tree_traverser_enter(&decl_list_stt, &decl_var_decl_stt);

                    syntax_tree_traverser_seek_terminal(&decl_var_decl_stt, token_number_identifier());
                    token_t * identifier_node = &jcg->syntax_tree->token_buffer->tokens[decl_var_decl_stt.current_node->terminal.position];

                    log_printf("Got global identifier with name \"%s\"\n", identifier_node->identifier_data->name);

                    globals[global_count].name = identifier_node->identifier_data->name;
                    globals[global_count].type = decl_type;
                    globals[global_count].scope = decl_scope;

                    if (syntax_tree_traverser_seek_nonterminal(&decl_var_decl_stt, NT_DECL_VAR_DECL_EQUAL)) {
                        syntax_tree_traverser_enter(&decl_var_decl_stt, &globals[global_count].expression_stt);

                        syntax_tree_traverser_seek_nonterminal(&globals[global_count].expression_stt, NT_EXPRESSION_LEVEL_14);
                    }

                    global_count++;
                    if (global_count == global_capacity) {
                        global_capacity *= 2;

                        globals = pkcc_realloc(globals, global_capacity * sizeof(java_generator_global_t));
                    }

                    if (!syntax_tree_traverser_seek_nonterminal(&decl_list_stt, NT_DECL_VAR_LIST)) break;

                    syntax_tree_traverser_enter_self(&decl_list_stt);
                }
            }
        }

        if (!syntax_tree_traverser_seek_nonterminal(&start_stt, NT_START)) break;

        syntax_tree_traverser_enter_self(&start_stt);
    }

    for (size_t i = 0; i < global_count; i++) {
        char * type_str = java_type_stringify(globals[i].type);

        fprintf(jcg->out_file, "\t.field public static %s %s\n\n", globals[i].name, type_str);

        pkcc_free(type_str);
    }

    for (size_t i = 0; i < global_count; i++) {
        java_variable_registry_push_static(&jcg->jvr, globals[i].name, globals[i].type, globals[i].scope);
    }

    fprintf(
        jcg->out_file,
        "\t.method <clinit> : ()V\n"
        "\t.code stack 1024 locals 1024\n"
    );
    for (size_t i = 0; i < global_count; i++) {
        java_generator_expression(jcg, globals[i].expression_stt);

        char * type_str = java_type_stringify(globals[i].type);

        fprintf(
            jcg->out_file,
            "\t\tputstatic %s/%s %s\n",
            jcg->classname,
            globals[i].name,
            type_str
        );

        pkcc_free(type_str);
    }
    fprintf(
        jcg->out_file,
        "\t\treturn\n"
        "\t.end code\n"
        "\t.end method\n\n"
    );

    pkcc_free(globals);
}