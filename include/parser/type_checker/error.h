#pragma once

#include <debug/error_handler.h>

__NORETURN static inline void type_checker_error() {
    fatal_error("Internal type checker assertion failure\n");
}