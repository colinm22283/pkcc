#include <parser/rules.h>
#include <parser/type_checker/type_checker.h>

#include <debug/log.h>

void type_checker_init(type_checker_t * tc, syntax_tree_t * syntax_tree, token_buffer_t * token_buffer) {
    log_printf("Initializing type checker\n");

    tc->syntax_tree = syntax_tree;
    tc->token_buffer = token_buffer;

    type_checker_registry_init(&tc->type_registry);
}

void type_checker_free(type_checker_t * tc) {
    log_printf("Freeing type checker\n");

    type_checker_registry_free(&tc->type_registry);
}

void type_checker_run_recur(type_checker_t * tc, syntax_tree_node_t * node) {
    syntax_tree_node_list_node_t * list_node = node->head->next;

    while (list_node != node->tail) {
        if (list_node->token_type == RT_NONTERMINAL) {
            switch (list_node->nonterminal.nonterminal) {
                case NT_STRUCT: {
                    type_checker_registry_parse(&tc->type_registry, tc->token_buffer, list_node);
                } break;

                case NT_DECL_VAR: {
                    list_node->type = type_checker_registry_parse(
                        &tc->type_registry,
                        tc->token_buffer,
                        list_node->nonterminal.tree.head->next
                    );
                } break;

                default: {
                    type_checker_run_recur(tc, &list_node->nonterminal.tree);
                } break;
            }
        }

        list_node = list_node->next;
    }
}

void type_checker_run(type_checker_t * tc) {
    type_checker_run_recur(tc, &tc->syntax_tree->head);
}