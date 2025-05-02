#pragma once

#include <free_list.h>
#include <defs.h>

extern free_list_t free_list;

__NORETURN void exit_and_free(int exit_code);