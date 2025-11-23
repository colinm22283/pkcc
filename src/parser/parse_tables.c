#include <parser/parse_tables.h>
#include <parser/rule_registry.h>

#include <grammar/nonterminals.h>

#include <alloc.h>

bool first_table_row_contains(parse_tables_first_node_t * node, token_number_t terminal) {
    for (size_t i = 0; i < node->element_count; i++) {
        if (node->elements[i] == terminal) return true;
    }

    return false;
}

void first_table_recur(parse_tables_first_node_t * first_table, nonterminal_t nonterminal) {
    log_printf("Parsing \"%s\" for first table\n", rules_nonterminal_name(nonterminal));

    first_table[nonterminal].element_count = 0;
    first_table[nonterminal].elements = pkcc_alloc(1);

    rule_registry_result_t rule_result = rule_registry_lookup(nonterminal);

    for (size_t i = 0; i < rule_result.rule_count; i++) {
        rule_t * rule = rule_result.rules[i];

        switch (rule->tokens[0].type) {
            case RT_NONTERMINAL: {
                if (first_table->elements == NULL) {
                    first_table_recur(first_table, rule->tokens[0].nonterminal);
                }

                for (size_t j = 0; j < first_table[rule->tokens[0].nonterminal].element_count; j++) {
                    if (!first_table_row_contains(&first_table[nonterminal], first_table[rule->tokens[0].nonterminal].elements[j])) {
                        first_table[nonterminal].element_count++;
                        first_table[nonterminal].elements = pkcc_realloc(first_table[nonterminal].elements, first_table[nonterminal].element_count * sizeof(token_number_t));

                        first_table[nonterminal].elements[first_table[nonterminal].element_count - 1] = first_table[rule->tokens[0].nonterminal].elements[j];
                    }
                }
            } break;

            case RT_TERMINAL: {
                if (!first_table_row_contains(&first_table[nonterminal], rule->tokens[0].terminal)) {
                    first_table[nonterminal].element_count++;
                    first_table[nonterminal].elements = pkcc_realloc(first_table[nonterminal].elements, first_table[nonterminal].element_count * sizeof(token_number_t));

                    first_table[nonterminal].elements[first_table[nonterminal].element_count - 1] = rule->tokens[0].terminal;
                }
            } break;

            default: break;
        }
    }

    rule_registry_result_free(&rule_result);
}

void parse_tables_init(parse_tables_t * parse_tables) {
    parse_tables->first_node_count = NT_COUNT;
    parse_tables->first_nodes = pkcc_alloc(parse_tables->first_node_count * sizeof(parse_tables_first_node_t));
    for (size_t i = 0; i < parse_tables->first_node_count; i++) {
        parse_tables->first_nodes[i].element_count = 0;
        parse_tables->first_nodes[i].elements = NULL;
    }
}

void parse_tables_free(parse_tables_t * parse_tables) {
    for (size_t i = 0; i < parse_tables->first_node_count; i++) {
        if (parse_tables->first_nodes[i].elements != NULL) {
            pkcc_free(parse_tables->first_nodes[i].elements);
        }
    }
    pkcc_free(parse_tables->first_nodes);
}

void parse_tables_load(parse_tables_t * parse_tables) {
    for (size_t i = 0; i < parse_tables->first_node_count; i++) {
        first_table_recur(parse_tables->first_nodes, i);
    }

    
}

void parse_tables_print(parse_tables_t * parse_tables) {
    printf("-------------------------- FIRST(NT) --------------------------\n");
    printf("                            NT |                          FIRST\n");
    printf("-------------------------------|-------------------------------\n");

    for (size_t i = 0; i < parse_tables->first_node_count; i++) {
        printf("%30s | ", rules_nonterminal_name(i));

        for (size_t j = 0; j < parse_tables->first_nodes[i].element_count; j++) {
            printf("'%s' ", token_number_stringify(parse_tables->first_nodes[i].elements[j]));
        }

        printf("\n");
    }
}
