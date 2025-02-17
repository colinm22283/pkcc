#pragma once

#include <stdbool.h>
#include <stddef.h>

#include <preprocessor/variable_map.h>

bool preprocessor_evaluate(preprocessor_vmap_t * vmap, const char * expr, size_t len);