#pragma once

#include <stdbool.h>
#include <stddef.h>

struct type_checker_type_s;

typedef enum {
    BTS_SIGNED,
    BTS_UNSIGNED
} type_checker_base_type_sign_t;

typedef enum {
    BTT_VOID,
    BTT_BOOL,
    BTT_CHAR,
    BTT_SHORT,
    BTT_INT,
    BTT_LONG,
    BTT_LONG_LONG,
    BTT_FLOAT,
    BTT_DOUBLE,
} type_checker_base_type_type_t;

typedef struct {
    type_checker_base_type_sign_t sign;
    type_checker_base_type_type_t type;
} type_checker_base_type_t;


typedef enum {
    DTQ_CONST,
} type_checker_derived_type_qualifier_t;

typedef enum {
    DTT_QUALIFIED,
    DTT_POINTER,
    DTT_ARRAY,
    DTT_STRUCT,
} type_checker_derived_type_type_t;

typedef struct {
    type_checker_derived_type_type_t type;

    union {
        struct {
            size_t qualifier_count;
            type_checker_derived_type_qualifier_t * qualifiers;

            struct type_checker_type_s * subtype;
        } qualified;

        struct {
            struct type_checker_type_s * subtype;
        } pointer;

        struct {
            bool has_size;
            size_t size;

            struct type_checker_type_s * subtype;
        } array;

        struct {
            char * name;

            size_t subtype_count;
            struct type_checker_type_s ** subtypes;
        } structure;
    };
} type_checker_derived_type_t;

typedef struct type_checker_type_s {
    bool is_base;

    union {
        type_checker_base_type_t base_type;
        type_checker_derived_type_t derived_type;
    };
} type_checker_type_t;

void type_checker_type_free(type_checker_type_t * type);
char * type_checker_type_stringify(type_checker_type_t * type);

type_checker_type_t * type_checker_type_expand(type_checker_type_t * a, type_checker_type_t * b);

bool type_checker_type_equal(type_checker_type_t * a, type_checker_type_t * b);