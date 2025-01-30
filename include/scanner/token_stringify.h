#pragma once

#include <scanner/token.h>

#define TOKEN_STRINGIFY_BUFFER_REQUIREMENT (256)

void token_stringify(char * buffer, token_t * token);