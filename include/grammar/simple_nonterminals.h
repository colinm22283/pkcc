#pragma once

#include <stdbool.h>

#include <parser/rule.h>

enum {
    NT_TRANSLATION_UNIT,

    NT_LINE_LIST,
    NT_LINE,
    NT_NUM,
    NT_SUM,
    NT_MULT,

    NT_COUNT,

    NT_START,
    NT_NULL,
};

static inline const char * rules_nonterminal_name(nonterminal_t nonterminal) {
    switch (nonterminal) {
        case NT_TRANSLATION_UNIT: return "NT_TRANSLATION_UNIT";

        case NT_LINE_LIST: return "NT_LINE_LIST";
        case NT_LINE: return "NT_LINE";
        case NT_NUM: return "NT_NUM";
        case NT_SUM: return "NT_SUM";
        case NT_MULT: return "NT_MULT";

        case NT_START: return "NT_START";
        case NT_NULL: return "NT_NULL";

        default: return "UNKNOWN";
    }
}

static inline const char * rules_nonterminal_report_name(nonterminal_t nonterminal) {
    switch (nonterminal) {
        case NT_TRANSLATION_UNIT: return "translation unit";

        default: return "UNKNOWN";
    }
}
