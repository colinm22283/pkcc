#include <scanner/token.h>

#include <grammar/rules.h>
#include <grammar/nonterminals.h>

DEFINE_RULE_TOKENS(tokens_translation_unit_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_LINE_LIST)
);

DEFINE_RULE_TOKENS(tokens_line_list_0);

DEFINE_RULE_TOKENS(tokens_line_list_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_LINE),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_LINE_LIST)
);

DEFINE_RULE_TOKENS(tokens_line_0,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier()),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_DIRECT_ASSIGNMENT)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SUM),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_SEMICOLON))
);

DEFINE_RULE_TOKENS(tokens_sum_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_MULT),
);

DEFINE_RULE_TOKENS(tokens_sum_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_MULT),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_PLUS)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_SUM)
    );

DEFINE_RULE_TOKENS(tokens_mult_0,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_NUM),
);

DEFINE_RULE_TOKENS(tokens_mult_1,
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_NUM),
    DEFINE_TERMINAL_RULE_TOKEN(token_number_punctuation(SCANNER_PUNCTUATION_TYPE_STAR)),
    DEFINE_NONTERMINAL_RULE_TOKEN(NT_MULT)
);

DEFINE_RULE_TOKENS(tokens_num_0,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_identifier())
    );
DEFINE_RULE_TOKENS(tokens_num_1,
    DEFINE_TERMINAL_RULE_TOKEN(token_number_constant())
);

rule_t all_rules[RULE_COUNT] = {
    DEFINE_RULE_MULTI(NT_TRANSLATION_UNIT,
        tokens_translation_unit_0
    ),

    DEFINE_RULE_MULTI(NT_LINE_LIST,
        tokens_line_list_0,
        tokens_line_list_1
    ),

    DEFINE_RULE_MULTI(NT_LINE,
        tokens_line_0
    ),

    DEFINE_RULE_MULTI(NT_SUM,
        tokens_sum_0,
        tokens_sum_1
    ),

    DEFINE_RULE_MULTI(NT_MULT,
        tokens_mult_0,
        tokens_mult_1
    ),

    DEFINE_RULE_MULTI(NT_NUM,
        tokens_num_0,
        tokens_num_1
    ),
};
