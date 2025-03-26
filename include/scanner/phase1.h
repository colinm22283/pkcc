#pragma once

#include <scanner/scanner.h>

#define SCANNER_STRINGIFY_PHASE1_REQUIRED_LENGTH (1024)
const char * scanner_stringify_phase1(token_t * token);

void scanner_print_phase1(FILE * out_stream, scanner_t * scanner);