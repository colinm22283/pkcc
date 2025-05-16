#pragma once

#include <parser/type_checker/type_registry.h>
#include <parser/type_checker/variable_registry.h>

extern type_checker_type_t * java_getchar_type;
extern type_checker_type_t * java_putchar_type;
extern type_checker_type_t * java_getint_type;
extern type_checker_type_t * java_putint_type;
extern type_checker_type_t * java_getfloat_type;
extern type_checker_type_t * java_putfloat_type;
extern type_checker_type_t * java_putstring_type;

void java_add_lib_definitions(type_checker_registry_t * tr, type_checker_variable_registry_t * vr);