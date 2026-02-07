#include <parser/parse_tables.h>
#include <parser/rule_registry.h>

#include <grammar/nonterminals.h>

#include <alloc.h>

rule_token_t root_tokens[] = {
    { .type = RT_NONTERMINAL, .nonterminal = NT_TRANSLATION_UNIT, },
};
rule_t root_rule = { .nonterminal = NT_START, .token_count = 1, .tokens = root_tokens, };
rule_token_t end_token = { .type = RT_END, };

bool first_table_row_contains(parse_tables_first_node_t * node, token_number_t terminal) {
    for (size_t i = 0; i < node->element_count; i++) {
        if (node->elements[i].terminal == terminal) return true;
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
                    if (!first_table_row_contains(&first_table[nonterminal], first_table[rule->tokens[0].nonterminal].elements[j].terminal)) {
                        first_table[nonterminal].element_count++;
                        first_table[nonterminal].elements = pkcc_realloc(first_table[nonterminal].elements, first_table[nonterminal].element_count * sizeof(rule_token_t));

                        first_table[nonterminal].elements[first_table[nonterminal].element_count - 1] = first_table[rule->tokens[0].nonterminal].elements[j];
                    }
                }
            } break;

            case RT_TERMINAL: {
                if (!first_table_row_contains(&first_table[nonterminal], rule->tokens[0].terminal)) {
                    first_table[nonterminal].element_count++;
                    first_table[nonterminal].elements = pkcc_realloc(first_table[nonterminal].elements, first_table[nonterminal].element_count * sizeof(rule_token_t));

                    first_table[nonterminal].elements[first_table[nonterminal].element_count - 1] = rule->tokens[0];
                }
            } break;

            case RT_END: {
                bool found = false;
                for (size_t j = 0; j < first_table[nonterminal].element_count; j++) {
                    if (first_table[nonterminal].elements[j].type == RT_END) {
                        found = true;

                        break;
                    }
                }

                if (!found) {
                    first_table[nonterminal].element_count++;
                    first_table[nonterminal].elements = pkcc_realloc(first_table[nonterminal].elements, first_table[nonterminal].element_count * sizeof(rule_token_t));

                    first_table[nonterminal].elements[first_table[nonterminal].element_count - 1] = rule->tokens[0];
                }
            } break;

            default: break;
        }
    }

    rule_registry_result_free(&rule_result);
}

void first_table_follow_update(parse_state_production_t * prod, parse_tables_first_node_t * first) {
    if (prod->rule->token_count == 0) {
        bool found = false;
        for (size_t j = 0; j < first->element_count; j++) {
            if (rule_token_equal(&first->elements[j], prod->lookahead)) {
                found = true;

                break;
            }
        }

        if (!found) {
            first->element_count++;
            first->elements = pkcc_realloc(first->elements, first->element_count * sizeof(rule_token_t));

            first->elements[first->element_count - 1] = *prod->lookahead;
        }
    }
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

static inline parse_tables_follow_node_t * new_follow_node(parse_state_t * state, nonterminal_t nt) {
    state->follow_node_count++;
    state->follow_nodes = pkcc_realloc(state->follow_nodes, state->follow_node_count * sizeof(parse_tables_follow_node_t));

    parse_tables_follow_node_t * node = &state->follow_nodes[state->follow_node_count - 1];

    node->nonterminal = nt;

    node->token_count = 0;
    node->tokens = pkcc_alloc(1);

    return node;
}

static inline void follow_node_append(parse_tables_follow_node_t * follow, rule_token_t * tok) {
    for (size_t i = 0; i < follow->token_count; i++) {
        if (rule_token_equal(&follow->tokens[i], tok)) return;
    }

    follow->token_count++;
    follow->tokens = pkcc_realloc(follow->tokens, follow->token_count * sizeof(rule_token_t));

    memcpy(&follow->tokens[follow->token_count - 1], tok, sizeof(rule_token_t));
}

void follow_table_generate(parse_tables_t * parse_tables, parse_state_t * state) {
    for (size_t i = 0; i < state->production_count; i++) {
        parse_state_production_t * prod = state->productions[i];

        if (prod->position + 1 == prod->rule->token_count) {
            rule_token_t * token = &prod->rule->tokens[prod->position];

            if (token->type == RT_NONTERMINAL) {
                parse_tables_follow_node_t * dst = parse_table_follow_lookup(state, token->nonterminal);
                if (dst == NULL) {
                    dst = new_follow_node(state, token->nonterminal);
                }

                parse_tables_follow_node_t * src = parse_table_follow_lookup(state, prod->rule->nonterminal);

                if (src != NULL) {
                    for (size_t j = 0; j < src->token_count; j++) {
                        follow_node_append(dst, &src->tokens[j]);
                    }
                }
            }
        }

        if (prod->position < prod->rule->token_count - 1) {
            rule_token_t * token = &prod->rule->tokens[prod->position];
            rule_token_t * next_token = &prod->rule->tokens[prod->position + 1];

            if (token->type == RT_NONTERMINAL) {
                parse_tables_follow_node_t * dst = parse_table_follow_lookup(state, token->nonterminal);
                if (dst == NULL) {
                    dst = new_follow_node(state, token->nonterminal);
                }

                if (next_token->type == RT_NONTERMINAL) {
                    parse_tables_first_node_t * first = &parse_tables->first_nodes[next_token->nonterminal];

                    for (size_t j = 0; j < first->element_count; j++) {
                        follow_node_append(dst, &first->elements[j]);
                    }
                }
                else {
                    follow_node_append(dst, next_token);
                }
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

void close_state(parse_state_t * state) {
    for (size_t i = 0; i < state->production_count; i++) {
        parse_state_production_t * prod = state->productions[i];

        if (prod->position < prod->rule->token_count) {
            if (prod->rule->tokens[prod->position].type == RT_NONTERMINAL) {
                rule_registry_result_t rule_res = rule_registry_lookup(prod->rule->tokens[prod->position].nonterminal);

                for (size_t j = 0; j < rule_res.rule_count; j++) {
                    bool found = false;
                    for (size_t k = 0; k < state->production_count; k++) {
                        if (
                            state->productions[k]->rule == rule_res.rules[j] &&
                            state->productions[k]->position == 0
                        ) {
                            found = true;
                        }
                    }

                    if (!found) {
                        state->production_count++;
                        state->productions = pkcc_realloc(state->productions, state->production_count * sizeof(parse_state_production_t *));

                        parse_state_production_t * new_prod = state->productions[state->production_count - 1] = pkcc_alloc(sizeof(parse_state_production_t));

                        new_prod->lookahead = NULL;

                        new_prod->next = NULL_STATE;
                        new_prod->position = 0;
                        new_prod->rule = rule_res.rules[j];
                    }
                }

                rule_registry_result_free(&rule_res);
            }
        }
    }
}

void expand_state(parse_tables_t * parse_tables, parse_state_t * state) {
    size_t prod_count = state->production_count;

    for (size_t i = 0; i < prod_count; i++) {
        size_t token_count = 0;
        rule_token_t * tokens = NULL;

        if (state->productions[i]->position == state->productions[i]->rule->token_count) {
            parse_tables_follow_node_t * follow = parse_table_follow_lookup(state, state->productions[i]->rule->nonterminal);

            if (follow != NULL) {
                token_count = follow->token_count;
                tokens = follow->tokens;
            }
        }
        else if (state->productions[i]->position + 1 == state->productions[i]->rule->token_count) {
            parse_tables_follow_node_t * follow = parse_table_follow_lookup(state, state->productions[i]->rule->nonterminal);

            if (follow != NULL) {
                token_count = follow->token_count;
                tokens = follow->tokens;
            }
        }
        else {
            if (state->productions[i]->rule->tokens[state->productions[i]->position].type == RT_NONTERMINAL) {
                if (state->productions[i]->rule->tokens[state->productions[i]->position + 1].type == RT_TERMINAL) {
                    token_count = 1;
                    tokens = &state->productions[i]->rule->tokens[state->productions[i]->position + 1];
                }
                else {
                    parse_tables_first_node_t * first = &parse_tables->first_nodes[state->productions[i]->rule->tokens[state->productions[i]->position + 1].nonterminal];

                    token_count = first->element_count;
                    tokens = first->elements;
                }
            }
            else {
                if (state->productions[i]->rule->tokens[state->productions[i]->position + 1].type == RT_NONTERMINAL) {
                    parse_tables_first_node_t * first = &parse_tables->first_nodes[state->productions[i]->rule->tokens[state->productions[i]->position + 1].nonterminal];

                    token_count = first->element_count;
                    tokens = first->elements;
                }
                else {
                    token_count = 1;
                    tokens = &state->productions[i]->rule->tokens[state->productions[i]->position + 1];
                }
            }
        }

        if (tokens == NULL || token_count == 0) {
            state->productions[i]->lookahead = NULL;
        }
        else {
            state->productions[i]->lookahead = &tokens[0];

            for (size_t j = 1; j < token_count; j++) {
                state->production_count++;
                state->productions = pkcc_realloc(state->productions, state->production_count * sizeof(parse_state_production_t *));

                parse_state_production_t * new_prod = state->productions[state->production_count - 1] = pkcc_alloc(sizeof(parse_state_production_t));

                new_prod->lookahead = &tokens[j];

                new_prod->next = NULL_STATE;
                new_prod->position = state->productions[i]->position;
                new_prod->rule = state->productions[i]->rule;
            }
        }
    }
}

bool state_equal(parse_state_t * a, parse_state_t * b) {
    if (a->production_count != b->production_count) return false;

    for (size_t i = 0; i < a->production_count; i++) {
        if (
            a->productions[i]->position == b->productions[i]->position &&
            a->productions[i]->rule->nonterminal == b->productions[i]->rule->nonterminal
        ) {
            if (a->productions[i]->rule->token_count != b->productions[i]->rule->token_count) return false;

            for (size_t j = 0; j < a->productions[i]->rule->token_count; j++) {
                if (!rule_token_equal(&a->productions[i]->rule->tokens[j], &b->productions[i]->rule->tokens[j])) return false;
            }
        }
        else {
            return false;
        }
    }

    return true;
}

bool production_equal(parse_state_production_t * a, parse_state_production_t * b) {
    if (a->rule->nonterminal != b->rule->nonterminal) return false;

    if (a->position != b->position) return false;

    if (a->rule->token_count != b->rule->token_count) return false;

    for (size_t i = 0; i < a->rule->token_count; i++) {
        if (!rule_token_equal(&a->rule->tokens[i], &b->rule->tokens[i])) return false;
    }

    return true;
}

bool production_equal_adv(parse_state_production_t * target, parse_state_production_t * adv) {
    if (target->rule->nonterminal != adv->rule->nonterminal) return false;

    if (target->position != adv->position + 1) return false;

    if (target->rule->token_count != adv->rule->token_count) return false;

    for (size_t i = 0; i < target->rule->token_count; i++) {
        if (!rule_token_equal(&target->rule->tokens[i], &adv->rule->tokens[i])) return false;
    }

    return true;
}

void eval_states_recur(parse_tables_t * parse_tables, parse_state_t * state, size_t depth) {
    if (depth == 1000) fatal_error("Hit maximum eval state limit\n");

    close_state(state);

    follow_table_generate(parse_tables, state);

    expand_state(parse_tables, state);

    // parse_tables_print(parse_tables);

    for (size_t i = 0; i < state->production_count; i++) {
        parse_state_production_t * prod = state->productions[i];

        first_table_follow_update(prod, &parse_tables->first_nodes[prod->rule->nonterminal]);
    }

    follow_table_generate(parse_tables, state);

    size_t token_count = 0;
    rule_token_t ** tokens = pkcc_alloc(1);

    for (size_t i = 0; i < state->production_count; i++) {
        parse_state_production_t * prod = state->productions[i];

        if (prod->position < prod->rule->token_count) {
            bool exists = false;
            for (size_t j = 0; j < token_count; j++) {
                if (rule_token_equal(tokens[j], &prod->rule->tokens[prod->position])) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                token_count++;
                tokens = pkcc_realloc(tokens, token_count * sizeof(rule_token_t *));

                tokens[token_count - 1] = &prod->rule->tokens[prod->position];
            }
        }
    }

    size_t recur_count = 0, recur_capacity = 1;
    parse_state_t ** recur_states = pkcc_alloc(recur_capacity * sizeof(parse_state_t *));

    // for (size_t i = 0; i < token_count; i++) {
    for (size_t j = 0; j < state->production_count; j++) {
        if (depth == 0) {
            log_printf("%zu/%zu\n", j + 1, state->production_count);
        }

        parse_state_production_t * prod = state->productions[j];

        if (prod->position < prod->rule->token_count) {
            parse_state_t * target_state = NULL;

            for (size_t k = 0; k < parse_tables->state_count; k++) {
                parse_state_t * test_state = parse_tables->parse_states[k];

                target_state = NULL;

                for (size_t l = 0; l < test_state->production_count; l++) {
                    if (production_equal_adv(test_state->productions[l], prod)) {
                        target_state = test_state;

                        break;
                    }
                }

                if (target_state != NULL) {
                    break;
                }
            }

            if (target_state == NULL) {
                if (prod->rule->tokens[prod->position].type == RT_NONTERMINAL) {
                    for (size_t k = 0; k < state->production_count; k++) {
                        if (
                            rule_token_equal(&prod->rule->tokens[prod->position], &state->productions[k]->rule->tokens[prod->position]) &&
                            rule_token_equal(prod->lookahead, state->productions[k]->lookahead)
                        ) {
                            if (state->productions[k]->next != NULL_STATE) {
                                target_state = parse_tables->parse_states[state->productions[k]->next];

                                break;
                            }
                        }
                    }
                }

                if (target_state == NULL) target_state = parse_tables_add_state(parse_tables);

                target_state->production_count++;
                target_state->productions = pkcc_realloc(target_state->productions, target_state->production_count * sizeof(parse_state_production_t *));

                parse_state_production_t * new_prod = target_state->productions[target_state->production_count - 1] = pkcc_alloc(sizeof(parse_state_production_t));

                memcpy(new_prod, prod, sizeof(parse_state_production_t));
                new_prod->position++;
                new_prod->lookahead = NULL;

                parse_tables_follow_node_t * new_follow = new_follow_node(target_state, new_prod->rule->nonterminal);
                parse_tables_follow_node_t * old_follow = parse_table_follow_lookup(state, new_prod->rule->nonterminal);
                for (size_t k = 0; k < old_follow->token_count; k++) {
                    follow_node_append(new_follow, &old_follow->tokens[k]);
                }

                recur_states[recur_count++] = target_state;

                if (recur_count == recur_capacity) {
                    recur_capacity *= 2;

                    recur_states = pkcc_realloc(recur_states, recur_capacity * sizeof(parse_state_t *));
                }
            }

            prod->next = target_state->index;
        }
    }
    // }

    for (size_t i = 0; i < recur_count; i++) {
        eval_states_recur(parse_tables, recur_states[i], depth + 1);
    }

    pkcc_free(recur_states);

    pkcc_free(tokens);
}

static inline parse_state_action_t * action_get_reduce(parse_state_t * state, nonterminal_t nt) {
    for (size_t i = 0; i < state->action_count; i++) {
        parse_state_action_t * action = &state->actions[i];

        if (action->type == AT_REDUCE) {
            if (action->reduce.nonterminal == nt) {
                return action;
            }
        }
    }

    return NULL;
}

static inline parse_state_action_t * action_get_shift(parse_state_t * state, rule_token_t * tok, size_t next_state) {
    for (size_t i = 0; i < state->action_count; i++) {
        parse_state_action_t * action = &state->actions[i];

        if (action->type == AT_SHIFT) {
            if (rule_token_equal(action->shift.token, tok) && action->shift.next_state == next_state) {
                return action;
            }
        }
    }

    return NULL;
}

void actions_generate(parse_tables_t * parse_tables) {
    for (size_t i = 0; i < parse_tables->state_count; i++) {
        parse_state_t * state = parse_tables->parse_states[i];

        for (size_t j = 0; j < state->production_count; j++) {
            parse_state_production_t * prod = state->productions[j];

            if (prod->position == prod->rule->token_count) {
                parse_state_action_t * action = action_get_reduce(state, prod->rule->nonterminal);

                if (action == NULL) {
                    state->action_count++;

                    state->actions = pkcc_realloc(state->actions, state->action_count * sizeof(parse_state_action_t));

                    action = &state->actions[state->action_count - 1];

                    action->type = AT_REDUCE;

                    if (prod->lookahead == NULL) {
                        action->lookahead_count = 0;
                    }
                    else {
                        action->lookahead_count = 1;
                        action->lookaheads = pkcc_alloc(action->lookahead_count * sizeof(rule_token_t *));

                        action->lookaheads[0] = prod->lookahead;
                    }

                    action->reduce.nonterminal = prod->rule->nonterminal;
                    action->reduce.pop_count = prod->rule->token_count;
                }
                else {
                    if (action->lookahead_count != 0) {
                        action->lookahead_count++;
                        action->lookaheads = pkcc_realloc(action->lookaheads, action->lookahead_count * sizeof(rule_token_t *));

                        action->lookaheads[action->lookahead_count - 1] = prod->lookahead;
                    }
                }
            }
            else {
                parse_state_action_t * action = action_get_shift(state, &prod->rule->tokens[prod->position], prod->next);

                if (action == NULL) {
                    state->action_count++;

                    state->actions = pkcc_realloc(state->actions, state->action_count * sizeof(parse_state_action_t));

                    action = &state->actions[state->action_count - 1];

                    action->type = AT_SHIFT;

                    if (prod->lookahead == NULL) {
                        printf("TEST\n");
                        action->lookahead_count = 0;
                    }
                    else {
                        action->lookahead_count = 1;
                        action->lookaheads = pkcc_alloc(action->lookahead_count * sizeof(rule_token_t));
                        action->lookaheads[0] = prod->lookahead;
                    }

                    action->shift.token = &prod->rule->tokens[prod->position];
                    action->shift.next_state = prod->next;
                }
                else {
                    if (action->lookahead_count != 0) {
                        bool found = false;
                        for (size_t j = 0; j < action->lookahead_count; j++) {
                            if (rule_token_equal(action->lookaheads[j], prod->lookahead)) {
                                found = true;
                                break;
                            }
                        }

                        if (!found) {
                            action->lookahead_count++;
                            action->lookaheads = pkcc_realloc(action->lookaheads, action->lookahead_count * sizeof(rule_token_t *));

                            action->lookaheads[action->lookahead_count - 1] = prod->lookahead;
                        }
                    }
                }
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

    log_printf("First table generated\n");

    parse_state_t * root_state = parse_tables_add_state(parse_tables);

    root_state->production_count = 1;
    root_state->productions = pkcc_realloc(root_state->productions, root_state->production_count * sizeof(parse_state_production_t *));

    parse_state_production_t * root_production = root_state->productions[0] = pkcc_alloc(sizeof(parse_state_production_t));

    root_production->position = 0;
    root_production->rule = &root_rule;

    follow_node_append(new_follow_node(root_state, NT_START), &end_token);

    log_printf("Begin eval states\n");

    eval_states_recur(parse_tables, root_state, 0);

    log_printf("States evaluated\n");

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

            if (prod->lookahead != NULL) {
                switch (prod->lookahead->type) {
                    case RT_TERMINAL: {
                        printf("'%s' ", token_number_stringify(prod->lookahead->terminal));
                    } break;

                    case RT_NONTERMINAL: {
                        printf("'%s' ", rules_nonterminal_name(prod->lookahead->nonterminal));
                    } break;

                    case RT_END: {
                        printf("END ");
                    } break;

                    default: {
                        printf("invalid ");
                    } break;
                }
            }
            else {
                printf("ALL ");
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
            if (parse_tables->first_nodes[i].elements[j].type == RT_END) {
                printf("'$' ");
            }
            else printf("'%s' ", token_number_stringify(parse_tables->first_nodes[i].elements[j].terminal));
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

            printf("(%zu) ", action->lookahead_count);

            for (size_t k = 0; k < action->lookahead_count; k++) {
                switch (action->lookaheads[k]->type) {
                    case RT_TERMINAL: {
                        printf("'%s'", token_number_stringify(action->lookaheads[k]->terminal));
                    } break;

                    case RT_NONTERMINAL: {
                        printf("'%s'", rules_nonterminal_name(action->lookaheads[k]->nonterminal));
                    } break;

                    case RT_END: {
                        printf("'$'");
                    } break;

                    default: {
                        printf("'invalid'");
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
