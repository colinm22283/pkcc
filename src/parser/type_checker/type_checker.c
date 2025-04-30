#include <parser/rules.h>
#include <parser/type_checker/type_checker.h>

#include <debug/log.h>

void type_checker_init(type_checker_t * tc, line_buffer_t * line_buffer, syntax_tree_t * syntax_tree, token_buffer_t * token_buffer) {
    log_printf("Initializing type checker\n");

    tc->syntax_tree = syntax_tree;
    tc->line_buffer = line_buffer;
    tc->token_buffer = token_buffer;

    type_checker_scope_registry_init(&tc->scope_registry, syntax_tree);
    type_checker_variable_registry_init(&tc->variable_registry);
    type_checker_registry_init(&tc->type_registry);
}

void type_checker_free(type_checker_t * tc) {
    log_printf("Freeing type checker\n");

    type_checker_registry_free(&tc->type_registry);
    type_checker_variable_registry_free(&tc->variable_registry);
    type_checker_scope_registry_free(&tc->scope_registry);
}

void type_checker_run_recur_phase1(type_checker_t * tc, syntax_tree_node_t * node) {
    syntax_tree_node_list_node_t * list_node = node->head->next;

    while (list_node != node->tail) {
        if (list_node->token_type == RT_NONTERMINAL) {
            type_checker_run_recur_phase1(tc, &list_node->nonterminal.tree);

            if (
                list_node->nonterminal.nonterminal == NT_TYPE ||
                list_node->nonterminal.nonterminal == NT_TYPE_VOID ||
                list_node->nonterminal.nonterminal == NT_STRUCT ||
                list_node->nonterminal.nonterminal == NT_DECL_FUNC
            ) list_node->type = type_checker_registry_parse(&tc->type_registry, &tc->variable_registry, tc->line_buffer, tc->token_buffer, list_node);
        }
        else list_node->type = type_checker_registry_parse(&tc->type_registry, &tc->variable_registry, tc->line_buffer, tc->token_buffer, list_node);

        list_node = list_node->next;
    }
}

void type_checker_run_recur_phase2(type_checker_t * tc, syntax_tree_node_t * node) {
    syntax_tree_node_list_node_t * list_node = node->head->next;

    while (list_node != node->tail) {
        if (list_node->token_type == RT_NONTERMINAL) {
            type_checker_run_recur_phase2(tc, &list_node->nonterminal.tree);

            if (
                list_node->nonterminal.nonterminal != NT_TYPE &&
                list_node->nonterminal.nonterminal != NT_TYPE_VOID &&
                list_node->nonterminal.nonterminal != NT_STRUCT &&
                list_node->nonterminal.nonterminal != NT_DECL_FUNC
            ) list_node->type = type_checker_registry_parse(&tc->type_registry, &tc->variable_registry, tc->line_buffer, tc->token_buffer, list_node);
        }

        list_node = list_node->next;
    }
}

void type_checker_run(type_checker_t * tc) {
    type_checker_scope_registry_run(&tc->scope_registry);

    if (options.dump_tree) syntax_tree_print(tc->syntax_tree);

    type_checker_run_recur_phase1(tc, &tc->syntax_tree->head);

    type_checker_run_recur_phase2(tc, &tc->syntax_tree->head);

    type_verifier_run(tc->line_buffer, tc->token_buffer, tc->syntax_tree);
}