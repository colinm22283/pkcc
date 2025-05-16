#include <parser/rules.h>

#include <java/java_default_value.h>
#include <java/generators/code_list.h>
#include <java/generators/code_line.h>

java_variable_registry_node_t * java_generator_code_list(
    java_code_generator_t * jcg,
    syntax_tree_traverser_t stt,
    type_checker_type_t * function_type,
    java_label_t * break_label,
    java_label_t * continue_label
) {
    syntax_tree_traverser_t code_list_stt;
    syntax_tree_traverser_enter(&stt, &code_list_stt);

    while (true) {
        syntax_tree_traverser_seek_nonterminal(&code_list_stt, NT_CODE_LINE);

        java_generator_code_line(jcg, code_list_stt, function_type, break_label, continue_label);

        if (!syntax_tree_traverser_seek_nonterminal(&code_list_stt, NT_CODE_LIST)) break;

        syntax_tree_traverser_enter_self(&code_list_stt);
    }
}