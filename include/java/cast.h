#pragma once

#include <parser/type_checker/type.h>

#include <java/code_generator.h>

bool java_cast(java_code_generator_t * jcg, type_checker_type_t * target_type, type_checker_type_t * source_type);