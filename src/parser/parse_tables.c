#include <parser/parse_tables.h>
#include <parser/rule_registry.h>

#include <grammar/nonterminals.h>

#include <alloc.h>

rule_token_t root_tokens[] = {
    { .type = RT_NONTERMINAL, .nonterminal = NT_TRANSLATION_UNIT, }
};
rule_t root_rule = { .nonterminal = NT_START, .token_count = 1, .tokens = root_tokens, };
rule_token_t root_lookahead[] = {
    { .type = RT_END, }
};

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
                if (first_table[rule->tokens[0].nonterminal].elements == NULL) {
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

void follow_table_recur(parse_tables_t * parse_tables, parse_state_t * state, parse_state_production_t * prod) {
    if (prod->rule->tokens[prod->rule->token_count - 1].type == RT_NONTERMINAL) {
        parse_tables_follow_node_t * parent_follow = parse_table_follow_lookup(state, prod->rule->nonterminal);
        parse_tables_follow_node_t * child_follow = parse_table_follow_lookup(state, prod->rule->tokens[prod->rule->token_count - 1].nonterminal);

        if (parent_follow != NULL && child_follow != NULL) {
            for (size_t k = 0; k < parent_follow->token_count; k++) {
                rule_token_t * parent_tok = &parent_follow->tokens[k];

                bool skip = false;
                for (size_t m = 0; m < child_follow->token_count; m++) {
                    if (rule_token_equal(parent_tok, &child_follow->tokens[m])) {
                        skip = true;
                        break;
                    }
                }

                if (!skip) {
                    child_follow->token_count++;

                    child_follow->tokens = pkcc_realloc(child_follow->tokens, child_follow->token_count * sizeof(rule_token_t));

                    child_follow->tokens[child_follow->token_count - 1].type = parent_tok->type;
                    child_follow->tokens[child_follow->token_count - 1].terminal = parent_tok->terminal;

                    for (size_t l = 0; l < state->production_count; l++) {
                        if (state->productions[l]->rule->nonterminal == prod->rule->tokens[prod->rule->token_count - 1].nonterminal) {
                            follow_table_recur(parse_tables, state, state->productions[l]);
                        }
                    }
                }
            }
        }
    }
}

void follow_table_generate(parse_tables_t * parse_tables) {
    for (size_t i = 0; i < parse_tables->state_count; i++) {
        parse_state_t * state = parse_tables->parse_states[i];

        size_t nt_count = 0;
        nonterminal_t * nts = pkcc_alloc(1);

        for (size_t j = 0; j < state->production_count; j++) {
            parse_state_production_t * prod = state->productions[j];

            for (size_t k = 0; k < prod->rule->token_count; k++) {
                rule_token_t * token = &prod->rule->tokens[k];

                if (token->type == RT_NONTERMINAL) {
                    bool skip = false;
                    for (size_t l = 0; l < nt_count; l++) {
                        if (nts[l] == token->nonterminal) {
                            skip = true;
                            break;
                        }
                    }

                    if (!skip) {
                        nt_count++;

                        nts = pkcc_realloc(nts, nt_count * sizeof(nonterminal_t));

                        nts[nt_count - 1] = token->nonterminal;
                    }
                }
            }
        }

        for (size_t j = 0; j < nt_count; j++) {
            state->follow_node_count++;

            state->follow_nodes = pkcc_realloc(state->follow_nodes, state->follow_node_count * sizeof(parse_tables_follow_node_t));

            parse_tables_follow_node_t * follow = &state->follow_nodes[state->follow_node_count - 1];

            follow->nonterminal = nts[j];
            follow->token_count = 0;
            follow->tokens = pkcc_alloc(1);

            for (size_t k = 0; k < state->production_count; k++) {
                if (state->productions[k]->position + 1 < state->productions[k]->rule->token_count) {
                    rule_token_t * token = &state->productions[k]->rule->tokens[state->productions[k]->position];
                    rule_token_t * next_token = &state->productions[k]->rule->tokens[state->productions[k]->position + 1];

                    if (token->type == RT_NONTERMINAL && token->nonterminal == nts[j]) {
                        switch (next_token->type) {
                            case RT_TERMINAL: {
                                bool skip = false;
                                for (size_t m = 0; m < follow->token_count; m++) {
                                    if (follow->tokens[m].type == RT_TERMINAL && follow->tokens[m].terminal == next_token->terminal) {
                                        skip = true;
                                        break;
                                    }
                                }

                                if (!skip) {
                                    follow->token_count++;

                                    follow->tokens = pkcc_realloc(follow->tokens, follow->token_count * sizeof(rule_token_t));

                                    follow->tokens[follow->token_count - 1].type = RT_TERMINAL;
                                    follow->tokens[follow->token_count - 1].terminal = next_token->terminal;
                                }
                            } break;

                            case RT_NONTERMINAL: {
                                parse_tables_first_node_t * first = &parse_tables->first_nodes[next_token->nonterminal];

                                for (size_t l = 0; l < first->element_count; l++) {
                                    bool skip = false;
                                    for (size_t m = 0; m < follow->token_count; m++) {
                                        if (follow->tokens[m].type == RT_TERMINAL && follow->tokens[m].terminal == first->elements[l]) {
                                            skip = true;
                                            break;
                                        }
                                    }

                                    if (!skip) {
                                        follow->token_count++;

                                        follow->tokens = pkcc_realloc(follow->tokens, follow->token_count * sizeof(rule_token_t));

                                        follow->tokens[follow->token_count - 1].type = RT_TERMINAL;
                                        follow->tokens[follow->token_count - 1].terminal = first->elements[l];
                                    }
                                }
                            } break;

                            case RT_END: {
                                bool skip = false;
                                for (size_t m = 0; m < follow->token_count; m++) {
                                    if (follow->tokens[m].type == RT_END) {
                                        skip = true;
                                        break;
                                    }
                                }

                                if (!skip) {
                                    follow->token_count++;

                                    follow->tokens = pkcc_realloc(follow->tokens, follow->token_count * sizeof(rule_token_t));

                                    follow->tokens[follow->token_count - 1].type = RT_END;
                                }
                            } break;
                        }
                    }
                }
            }
        }

        pkcc_free(nts);
    }

    for (size_t i = 0; i < parse_tables->state_count; i++) {
        parse_state_t * state = parse_tables->parse_states[i];

        for (size_t j = 0; j < state->production_count; j++) {
            parse_state_production_t * prod = state->productions[j];

            follow_table_recur(parse_tables, state, prod);
        }
    }

    for (size_t i = 0; i < parse_tables->state_count; i++) {
        parse_state_t * state = parse_tables->parse_states[i];

        for (size_t j = 0; j < state->production_count; j++) {
            parse_state_production_t * prod = state->productions[j];

            parse_tables_follow_node_t * follow = parse_table_follow_lookup(state, prod->rule->nonterminal);

            if (follow == NULL) {
                prod->lookahead_count = 0;
                prod->lookaheads = NULL;
            }
            else {
                prod->lookahead_count = follow->token_count;
                prod->lookaheads = follow->tokens;
            }
        }
    }
}

parse_state_t * parse_tables_add_state(parse_tables_t * parse_tables) {
    parse_state_t * new_state = parse_tables->parse_states[parse_tables->state_count++] = pkcc_alloc(sizeof(parse_state_t));

    if (parse_tables->state_count == parse_tables->state_capacity) {
        parse_tables->state_capacity *= 2;

        parse_tables->parse_states = pkcc_realloc(parse_tables->parse_states, parse_tables->state_capacity * sizeof(parse_state_t *));
    }

    new_state->index = parse_tables->state_count - 1;

    new_state->production_count = 0;
    new_state->productions = pkcc_alloc(1);

    new_state->follow_node_count = 0;
    new_state->follow_nodes = pkcc_alloc(1);

    new_state->action_count = 0;
    new_state->actions = pkcc_alloc(1);

    return new_state;
}

void eval_states_recur(parse_tables_t * parse_tables, parse_state_t * state, size_t depth) {
    if (depth == 1000) {
        fatal_error("State evaluation limit reached!\n");
    }

    size_t nt_count = 0;
    nonterminal_t * read_nts = pkcc_alloc(1);

    for (size_t i = 0; i < state->production_count; i++) {
        rule_t * rule = state->productions[i]->rule;

        if (state->productions[i]->position < rule->token_count) {
            rule_token_t * tok = &rule->tokens[state->productions[i]->position];

            switch (tok->type) {
                case RT_NONTERMINAL: {
                    bool already_checked = false;
                    for (size_t j = 0; j < nt_count; j++) {
                        if (read_nts[j] == tok->nonterminal) {
                            already_checked = true;
                            break;
                        }
                    }
                    if (already_checked) break;

                    rule_registry_result_t reg_res = rule_registry_lookup(tok->nonterminal);

                    nt_count++;
                    read_nts = pkcc_realloc(read_nts, nt_count * sizeof(nonterminal_t));

                    read_nts[nt_count - 1] = tok->nonterminal;

                    for (size_t j = 0; j < reg_res.rule_count; j++) {
                        bool skip = false;
                        for (size_t k = 0; k < state->production_count; k++) {
                            if (state->productions[k]->rule == reg_res.rules[j]) {
                                skip = true;

                                break;
                            }
                        }

                        if (!skip) {
                            parse_state_production_t * prod = pkcc_alloc(sizeof(parse_state_production_t));

                            prod->position = 0;
                            prod->next = NULL_STATE;
                            prod->rule = reg_res.rules[j];

                            prod->lookahead_count = 0;
                            prod->lookaheads = pkcc_alloc(1);

                            state->production_count++;
                            state->productions = pkcc_realloc(state->productions, state->production_count * sizeof(parse_state_production_t *));

                            state->productions[state->production_count - 1] = prod;
                        }
                    }

                    rule_registry_result_free(&reg_res);
                } break;

                default: break;
            }
        }
    }

    pkcc_free(read_nts);

    size_t state_count = 0;
    size_t * new_states = pkcc_alloc(1);

    for (size_t i = 0; i < state->production_count; i++) {
        parse_state_production_t * prod = state->productions[i];

        if (prod->position < prod->rule->token_count) {
            parse_state_t * new_state = NULL;

            for (size_t j = 0; j < i; j++) {
                parse_state_production_t * p = state->productions[j];

                if (
                    p->position < p->rule->token_count &&
                    rule_token_equal(&p->rule->tokens[p->position], &prod->rule->tokens[prod->position])
                ) {
                    new_state = parse_tables->parse_states[p->next];

                    break;
                }
            }

            if (new_state == NULL) {
                parse_state_t * identical_state = NULL;

                for (size_t j = 0; j < parse_tables->state_count; j++) {
                    for (size_t k = 0; k < parse_tables->parse_states[j]->production_count; k++) {
                        if (
                            parse_tables->parse_states[j]->productions[k]->rule == prod->rule &&
                            parse_tables->parse_states[j]->productions[k]->position == prod->position + 1
                        ) {
                            identical_state = parse_tables->parse_states[j];
                        }
                    }
                }

                if (identical_state != NULL) {
                    prod->next = identical_state->index;

                    continue;
                }
            }
            else {
                parse_state_t * identical_state = NULL;

                for (size_t k = 0; k < new_state->production_count; k++) {
                    if (
                        new_state->productions[k]->rule == prod->rule &&
                        new_state->productions[k]->position == prod->position + 1
                    ) {
                        identical_state = new_state;
                    }
                }

                if (identical_state != NULL) {
                    prod->next = identical_state->index;

                    continue;
                }
            }

            if (new_state == NULL) {
                new_state = parse_tables_add_state(parse_tables);

                state_count++;
                new_states = pkcc_realloc(new_states, state_count * sizeof(size_t));
                new_states[state_count - 1] = new_state->index;
            }

            new_state->production_count++;
            new_state->productions = pkcc_realloc(new_state->productions, new_state->production_count * sizeof(parse_state_production_t *));

            parse_state_production_t * new_prod = new_state->productions[new_state->production_count - 1] = pkcc_alloc(sizeof(parse_state_production_t));

            memcpy(new_prod, prod, sizeof(parse_state_production_t));

            new_prod->position++;
            prod->next = new_state->index;
        }
    }

    for (size_t i = 0; i < state_count; i++) {
        eval_states_recur(parse_tables, parse_tables->parse_states[new_states[i]], depth + 1);
    }


    pkcc_free(new_states);
}

void actions_generate(parse_tables_t * parse_tables) {
    for (size_t i = 0; i < parse_tables->state_count; i++) {
        parse_state_t * state = parse_tables->parse_states[i];

        for (size_t j = 0; j < state->production_count; j++) {
            parse_state_production_t * prod = state->productions[j];

            if (prod->position == prod->rule->token_count) {
                state->action_count++;

                state->actions = pkcc_realloc(state->actions, state->action_count * sizeof(parse_state_action_t));

                parse_state_action_t * action = &state->actions[state->action_count - 1];

                action->type = AT_REDUCE;
                action->lookahead_count = prod->lookahead_count;
                action->lookaheads = prod->lookaheads;

                action->reduce.nonterminal = prod->rule->nonterminal;
                action->reduce.pop_count = prod->rule->token_count;
            }
            else {
                state->action_count++;

                state->actions = pkcc_realloc(state->actions, state->action_count * sizeof(parse_state_action_t));

                parse_state_action_t * action = &state->actions[state->action_count - 1];

                action->type = AT_SHIFT;
                action->lookahead_count = prod->lookahead_count;
                action->lookaheads = prod->lookaheads;

                action->shift.token = &prod->rule->tokens[prod->position];
                action->shift.next_state = prod->next;
            }
        }
    }
}

void parse_tables_init(parse_tables_t * parse_tables) {
    parse_tables->first_node_count = NT_COUNT;
    parse_tables->first_nodes = pkcc_alloc(parse_tables->first_node_count * sizeof(parse_tables_first_node_t));
    for (size_t i = 0; i < parse_tables->first_node_count; i++) {
        parse_tables->first_nodes[i].element_count = 0;
        parse_tables->first_nodes[i].elements = NULL;
    }

    parse_tables->state_count = 0;
    parse_tables->state_capacity = 1;
    parse_tables->parse_states = pkcc_alloc(parse_tables->state_capacity * sizeof(parse_state_t *));
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

    parse_state_t * root_state = parse_tables_add_state(parse_tables);

    root_state->production_count = 1;
    root_state->productions = pkcc_realloc(root_state->productions, root_state->production_count * sizeof(parse_state_production_t *));

    parse_state_production_t * root_production = root_state->productions[0] = pkcc_alloc(sizeof(parse_state_production_t));

    root_production->position = 0;
    root_production->rule = &root_rule;

    root_production->lookahead_count = 1;
    root_production->lookaheads = root_lookahead;

    eval_states_recur(parse_tables, root_state, 0);

    follow_table_generate(parse_tables);

    actions_generate(parse_tables);
}

void parse_tables_print(parse_tables_t * parse_tables) {
    printf("--------------------------  STATES   --------------------------\n");

    for (size_t i = 0; i < parse_tables->state_count; i++) {
        printf("STATE %zu:\n", i);

        for (size_t j = 0; j < parse_tables->parse_states[i]->follow_node_count; j++) {
            printf("  FOLLOW %s: ", rules_nonterminal_name(parse_tables->parse_states[i]->follow_nodes[j].nonterminal));

            for (size_t k = 0; k < parse_tables->parse_states[i]->follow_nodes[j].token_count; k++) {
                switch (parse_tables->parse_states[i]->follow_nodes[j].tokens[k].type) {
                    case RT_TERMINAL: {
                        printf("%s ", token_number_stringify(parse_tables->parse_states[i]->follow_nodes[j].tokens[k].terminal));
                    } break;

                    case RT_NONTERMINAL: {
                        fatal_error("Received nonterminal in follow set\n");
                    } break;

                    case RT_END: {
                        printf("$ ");
                    } break;
                }
            }

            printf("\n");
        }

        printf("\n");

        for (size_t j = 0; j < parse_tables->parse_states[i]->production_count; j++) {
            parse_state_production_t * prod = parse_tables->parse_states[i]->productions[j];
            rule_t * rule = prod->rule;

            printf("  %30s -> ", rules_nonterminal_name(rule->nonterminal));

            for (size_t k = 0; k < rule->token_count; k++) {
                if (k == prod->position) {
                    printf("∘ ");
                }

                switch (rule->tokens[k].type) {
                    case RT_TERMINAL: {
                        printf("%s ", token_number_stringify(rule->tokens[k].terminal));
                    } break;

                    case RT_NONTERMINAL: {
                        printf("%s ", rules_nonterminal_name(rule->tokens[k].nonterminal));
                    } break;

                    case RT_END: {
                        printf("END ");
                    } break;
                }
            }

            if (rule->token_count == prod->position) {
                printf("∘");
            }

            if (prod->next != NULL_STATE) {
                printf("    NEXT = %zu", prod->next);
            }

            printf("    { ");
            
            for (size_t k = 0; k < prod->lookahead_count; k++) {
                switch (prod->lookaheads[k].type) {
                    case RT_TERMINAL: {
                        printf("%s ", token_number_stringify(prod->lookaheads[k].terminal));
                    } break;

                    case RT_NONTERMINAL: {
                        printf("%s ", rules_nonterminal_name(prod->lookaheads[k].nonterminal));
                    } break;

                    case RT_END: {
                        printf("END ");
                    } break;
                }
            }

            printf("}\n");
        }
    }

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

    printf("-------------------------- ACTIONS --------------------------\n");
    
    for (size_t i = 0; i < parse_tables->state_count; i++) {
        parse_state_t * state = parse_tables->parse_states[i];

        printf("%zu:\n", i);

        for (size_t j = 0; j < state->action_count; j++) {
            parse_state_action_t * action = &state->actions[j];

            printf("  ");

            switch (action->type) {
                case AT_REDUCE: {
                    printf("%s: REDUCE %zu", rules_nonterminal_name(action->reduce.nonterminal), action->reduce.pop_count);
                } break;

                case AT_SHIFT: {
                    switch (action->shift.token->type) {
                        case RT_TERMINAL: {
                            printf("%s", token_number_stringify(action->shift.token->terminal));
                        } break;

                        case RT_NONTERMINAL: {
                            printf("%s", rules_nonterminal_name(action->shift.token->nonterminal));
                        } break;

                        case RT_END: {
                            printf("$");
                        } break;
                    }

                    printf(": SHIFT %zu", action->shift.next_state);
                } break;
            }

            printf("        ");

            for (size_t k = 0; k < action->lookahead_count; k++) {
                switch (action->lookaheads[k].type) {
                    case RT_TERMINAL: {
                        printf("%s", token_number_stringify(action->lookaheads[k].terminal));
                    } break;

                    case RT_NONTERMINAL: {
                        printf("%s", rules_nonterminal_name(action->lookaheads[k].nonterminal));
                    } break;

                    case RT_END: {
                        printf("$");
                    } break;
                }

                printf(" ");
            }

            printf("\n");
        }

        printf("\n");
    }
}

parse_tables_follow_node_t * parse_table_follow_lookup(parse_state_t * state, nonterminal_t nt) {
    for (size_t i = 0; i < state->follow_node_count; i++) {
        if (state->follow_nodes[i].nonterminal == nt) {
            return &state->follow_nodes[i];
        }
    }

    return NULL;
}
