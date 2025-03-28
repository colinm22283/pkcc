#pragma once

#include <parser/rule.h>

#define RULE_COUNT (157)

enum {
    NT_START = 0,

    NT_DECL,

    NT_DECL_VAR,
    NT_DECL_VAR_NAME_LIST,
    NT_DECL_VAR_LIST,
    NT_DECL_VAR_DECL,
    NT_DECL_VAR_DECL_EQUAL,
    NT_DECL_VAR_DECL_ARRAY,

    NT_DECL_FUNC,
    NT_DECL_FUNC_ARG_LIST,
    NT_DECL_FUNC_ARG_LIST_EXTRA,

    NT_CODE_BLOCK,
    NT_CODE_LIST,
    NT_CODE_LINE,

    NT_STATEMENT,

    NT_STATEMENT_RETURN_VALUE,
    NT_STATEMENT_IF_ELSE,
    NT_STATEMENT_FOR_PARAM_1,

    NT_OPTIONAL_EXPRESSION,

    NT_EXPRESSION,
    NT_EXPRESSION_LEVEL_15,
    NT_EXPRESSION_LEVEL_15_PRIME,
    NT_EXPRESSION_LEVEL_14,
    NT_EXPRESSION_LEVEL_14_PRIME,
    NT_EXPRESSION_LEVEL_13,
    NT_EXPRESSION_LEVEL_13_PRIME,
    NT_EXPRESSION_LEVEL_12,
    NT_EXPRESSION_LEVEL_12_PRIME,
    NT_EXPRESSION_LEVEL_11,
    NT_EXPRESSION_LEVEL_11_PRIME,
    NT_EXPRESSION_LEVEL_10,
    NT_EXPRESSION_LEVEL_10_PRIME,
    NT_EXPRESSION_LEVEL_9,
    NT_EXPRESSION_LEVEL_9_PRIME,
    NT_EXPRESSION_LEVEL_8,
    NT_EXPRESSION_LEVEL_8_PRIME,
    NT_EXPRESSION_LEVEL_7,
    NT_EXPRESSION_LEVEL_7_PRIME,
    NT_EXPRESSION_LEVEL_6,
    NT_EXPRESSION_LEVEL_6_PRIME,
    NT_EXPRESSION_LEVEL_5,
    NT_EXPRESSION_LEVEL_5_PRIME,
    NT_EXPRESSION_LEVEL_4,
    NT_EXPRESSION_LEVEL_4_PRIME,
    NT_EXPRESSION_LEVEL_3,
    NT_EXPRESSION_LEVEL_3_PRIME,
    NT_EXPRESSION_LEVEL_2,
    NT_EXPRESSION_LEVEL_1,
    NT_EXPRESSION_LEVEL_1_PRIME,
    NT_EXPRESSION_LEVEL_0,

    NT_CALL_ARG_LIST,
    NT_CALL_ARG_LIST_EXTRA,

    NT_STRUCT,
    NT_STRUCT_MEMBER_LIST,
    NT_STRUCT_MEMBER,
    NT_STRUCT_NAME,

    NT_TYPE_VOID,
    NT_TYPE,
    NT_TYPE_INNER,

    NT_QUALIFIER,

    NT_NULL,
};

extern rule_t all_rules[RULE_COUNT];

static inline const char * rules_nonterminal_name(nonterminal_t nonterminal) {
    switch (nonterminal) {
        case NT_START: return "NT_START";

        case NT_DECL: return "NT_DECL";

        case NT_DECL_VAR: return "NT_DECL_VAR";
        case NT_DECL_VAR_NAME_LIST: return "NT_DECL_VAR_NAME_LIST";
        case NT_DECL_VAR_LIST: return "NT_DECL_VAR_LIST";
        case NT_DECL_VAR_DECL: return "NT_DECL_VAR_DECL";
        case NT_DECL_VAR_DECL_EQUAL: return "NT_DECL_VAR_DECL_EQUAL";
        case NT_DECL_VAR_DECL_ARRAY: return "NT_DECL_VAR_DECL_ARRAY";

        case NT_DECL_FUNC: return "NT_DECL_FUNC";
        case NT_DECL_FUNC_ARG_LIST: return "NT_DECL_FUNC_ARG_LIST";
        case NT_DECL_FUNC_ARG_LIST_EXTRA: return "NT_DECL_FUNC_ARG_LIST_EXTRA";

        case NT_CODE_BLOCK: return "NT_CODE_BLOCK";
        case NT_CODE_LIST: return "NT_CODE_LIST";
        case NT_CODE_LINE: return "NT_CODE_LINE";

        case NT_STATEMENT: return "NT_STATEMENT";

        case NT_STATEMENT_RETURN_VALUE: return "NT_STATEMENT_RETURN_VALUE";
        case NT_STATEMENT_IF_ELSE: return "NT_STATEMENT_IF_ELSE";
        case NT_STATEMENT_FOR_PARAM_1: return "NT_STATEMENT_FOR_PARAM_1";

        case NT_OPTIONAL_EXPRESSION: return "NT_OPTIONAL_EXPRESSION";

        case NT_EXPRESSION: return "NT_EXPRESSION";
        case NT_EXPRESSION_LEVEL_15: return "NT_EXPRESSION_LEVEL_15";
        case NT_EXPRESSION_LEVEL_15_PRIME: return "NT_EXPRESSION_LEVEL_15_PRIME";
        case NT_EXPRESSION_LEVEL_14: return "NT_EXPRESSION_LEVEL_14";
        case NT_EXPRESSION_LEVEL_14_PRIME: return "NT_EXPRESSION_LEVEL_14_PRIME";
        case NT_EXPRESSION_LEVEL_13: return "NT_EXPRESSION_LEVEL_13";
        case NT_EXPRESSION_LEVEL_13_PRIME: return "NT_EXPRESSION_LEVEL_13_PRIME";
        case NT_EXPRESSION_LEVEL_12: return "NT_EXPRESSION_LEVEL_12";
        case NT_EXPRESSION_LEVEL_12_PRIME: return "NT_EXPRESSION_LEVEL_12_PRIME";
        case NT_EXPRESSION_LEVEL_11: return "NT_EXPRESSION_LEVEL_11";
        case NT_EXPRESSION_LEVEL_11_PRIME: return "NT_EXPRESSION_LEVEL_11_PRIME";
        case NT_EXPRESSION_LEVEL_10: return "NT_EXPRESSION_LEVEL_10";
        case NT_EXPRESSION_LEVEL_10_PRIME: return "NT_EXPRESSION_LEVEL_10_PRIME";
        case NT_EXPRESSION_LEVEL_9: return "NT_EXPRESSION_LEVEL_9";
        case NT_EXPRESSION_LEVEL_9_PRIME: return "NT_EXPRESSION_LEVEL_9_PRIME";
        case NT_EXPRESSION_LEVEL_8: return "NT_EXPRESSION_LEVEL_8";
        case NT_EXPRESSION_LEVEL_8_PRIME: return "NT_EXPRESSION_LEVEL_8_PRIME";
        case NT_EXPRESSION_LEVEL_7: return "NT_EXPRESSION_LEVEL_7";
        case NT_EXPRESSION_LEVEL_7_PRIME: return "NT_EXPRESSION_LEVEL_7_PRIME";
        case NT_EXPRESSION_LEVEL_6: return "NT_EXPRESSION_LEVEL_6";
        case NT_EXPRESSION_LEVEL_6_PRIME: return "NT_EXPRESSION_LEVEL_6_PRIME";
        case NT_EXPRESSION_LEVEL_5: return "NT_EXPRESSION_LEVEL_5";
        case NT_EXPRESSION_LEVEL_5_PRIME: return "NT_EXPRESSION_LEVEL_5_PRIME";
        case NT_EXPRESSION_LEVEL_4: return "NT_EXPRESSION_LEVEL_4";
        case NT_EXPRESSION_LEVEL_4_PRIME: return "NT_EXPRESSION_LEVEL_4_PRIME";
        case NT_EXPRESSION_LEVEL_3: return "NT_EXPRESSION_LEVEL_3";
        case NT_EXPRESSION_LEVEL_3_PRIME: return "NT_EXPRESSION_LEVEL_3_PRIME";
        case NT_EXPRESSION_LEVEL_2: return "NT_EXPRESSION_LEVEL_2";
        case NT_EXPRESSION_LEVEL_1: return "NT_EXPRESSION_LEVEL_1";
        case NT_EXPRESSION_LEVEL_1_PRIME: return "NT_EXPRESSION_LEVEL_1_PRIME";
        case NT_EXPRESSION_LEVEL_0: return "NT_EXPRESSION_LEVEL_0";

        case NT_CALL_ARG_LIST: return "NT_CALL_ARG_LIST";
        case NT_CALL_ARG_LIST_EXTRA: return "NT_CALL_ARG_LIST_EXTRA";

        case NT_STRUCT: return "NT_STRUCT";
        case NT_STRUCT_MEMBER_LIST: return "NT_STRUCT_MEMBER_LIST";
        case NT_STRUCT_MEMBER: return "NT_STRUCT_MEMBER";
        case NT_STRUCT_NAME: return "NT_STRUCT_NAME";

        case NT_TYPE_VOID: return "NT_TYPE_VOID";
        case NT_TYPE: return "NT_TYPE";
        case NT_TYPE_INNER: return "NT_TYPE_INNER";

        case NT_QUALIFIER: return "NT_QUALIFIER";

        default: return "UNKNOWN";
    }
}

static inline const char * rules_nonterminal_report_name(nonterminal_t nonterminal) {
    switch (nonterminal) {
        case NT_START: return "start";

        case NT_DECL: return "declaration";

        case NT_DECL_VAR: return "declaration";
        case NT_DECL_VAR_LIST: return "variable declaration";
        case NT_DECL_VAR_DECL: return "variable declaration";
        case NT_DECL_VAR_DECL_EQUAL: return "variable declaration";
        case NT_DECL_VAR_DECL_ARRAY: return "variable declaration";

        case NT_DECL_FUNC: return "declaration";
        case NT_DECL_FUNC_ARG_LIST: return "function declaration";
        case NT_DECL_FUNC_ARG_LIST_EXTRA: return "function declaration";

        case NT_CODE_BLOCK: return "statement";
        case NT_CODE_LIST: return "statement";
        case NT_CODE_LINE: return "statement";

        case NT_STATEMENT: return "statement";

        case NT_STATEMENT_RETURN_VALUE: return "statement";
        case NT_STATEMENT_IF_ELSE: return "statement";
        case NT_STATEMENT_FOR_PARAM_1: return "statement";

        case NT_OPTIONAL_EXPRESSION: return "expression";

        case NT_EXPRESSION: return "expression";
        case NT_EXPRESSION_LEVEL_15: return "expression";
        case NT_EXPRESSION_LEVEL_15_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_14: return "expression";
        case NT_EXPRESSION_LEVEL_14_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_13: return "expression";
        case NT_EXPRESSION_LEVEL_13_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_12: return "expression";
        case NT_EXPRESSION_LEVEL_12_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_11: return "expression";
        case NT_EXPRESSION_LEVEL_11_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_10: return "expression";
        case NT_EXPRESSION_LEVEL_10_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_9: return "expression";
        case NT_EXPRESSION_LEVEL_9_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_8: return "expression";
        case NT_EXPRESSION_LEVEL_8_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_7: return "expression";
        case NT_EXPRESSION_LEVEL_7_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_6: return "expression";
        case NT_EXPRESSION_LEVEL_6_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_5: return "expression";
        case NT_EXPRESSION_LEVEL_5_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_4: return "expression";
        case NT_EXPRESSION_LEVEL_4_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_3: return "expression";
        case NT_EXPRESSION_LEVEL_3_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_2: return "expression";
        case NT_EXPRESSION_LEVEL_1: return "expression";
        case NT_EXPRESSION_LEVEL_1_PRIME: return "expression";
        case NT_EXPRESSION_LEVEL_0: return "expression";

        case NT_CALL_ARG_LIST: return "expression";
        case NT_CALL_ARG_LIST_EXTRA: return "expression";

        case NT_STRUCT: return "struct";
        case NT_STRUCT_MEMBER_LIST: return "struct";
        case NT_STRUCT_MEMBER: return "struct";
        case NT_STRUCT_NAME: return "struct";

        case NT_TYPE_VOID: return "type";
        case NT_TYPE: return "type";
        case NT_TYPE_INNER: return "type";

        case NT_QUALIFIER: return "qualifier";

        default: return "UNKNOWN";
    }
}