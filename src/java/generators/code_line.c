#include <parser/rules.h>

#include <java/java_default_value.h>
#include <java/generators/code_line.h>
#include <java/generators/statement.h>
#include <java/generators/local_var.h>

java_variable_registry_node_t * java_generator_code_line(
    java_code_generator_t * jcg,
    syntax_tree_traverser_t stt,
    type_checker_type_t * function_type,
    java_label_t * break_label,
    java_label_t * continue_label
) {
    syntax_tree_traverser_t code_line_stt;
    syntax_tree_traverser_enter(&stt, &code_line_stt);

    if (syntax_tree_traverser_seek_nonterminal(&code_line_stt, NT_STATEMENT)) {
        return java_generator_statement(jcg, code_line_stt, function_type, break_label, continue_label);
    }
    else if (syntax_tree_traverser_seek_nonterminal(&code_line_stt, NT_DECL_VAR)) {
        return java_generator_local_var(jcg, code_line_stt, function_type);
    }

    return NULL;
}