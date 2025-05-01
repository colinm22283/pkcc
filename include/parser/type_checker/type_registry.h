#pragma once

#include <stddef.h>

#include <parser/syntax_tree.h>
#include <parser/type_checker/type.h>
#include <parser/type_checker/variable_registry.h>

typedef struct {
    type_checker_scope_t * scope;
    type_checker_type_t type;
} type_checker_registry_entry_t;

typedef struct {
    size_t size, capacity;
    type_checker_registry_entry_t ** entries;
} type_checker_registry_t;

extern type_checker_type_t * void_type;
extern type_checker_type_t * bool_type;
extern type_checker_type_t * uchar_type;
extern type_checker_type_t * ushort_type;
extern type_checker_type_t * uint_type;
extern type_checker_type_t * ulong_type;
extern type_checker_type_t * ulonglong_type;
extern type_checker_type_t * schar_type;
extern type_checker_type_t * sshort_type;
extern type_checker_type_t * sint_type;
extern type_checker_type_t * slong_type;
extern type_checker_type_t * slonglong_type;
extern type_checker_type_t * float_type;
extern type_checker_type_t * double_type;
extern type_checker_type_t * schararr_type;

void type_checker_registry_init(type_checker_registry_t * tr);
void type_checker_registry_free(type_checker_registry_t * tr);

type_checker_type_t * type_checker_registry_parse(
    type_checker_registry_t * tr,
    type_checker_variable_registry_t * vr,
    line_buffer_t * line_buffer,
    token_buffer_t * token_buffer,
    syntax_tree_node_list_node_t * node
);