#pragma once

#include <stdio.h>

#include <line_buffer.h>
#include <scanner/token_buffer.h>

typedef struct {
    token_buffer_t tb;
} scanner_t;

void scanner_init(scanner_t * scanner);
void scanner_free(scanner_t * scanner);

void scanner_scan(scanner_t * scanner, line_buffer_t * line_buffer);

void scanner_print_tokens(scanner_t * scanner, line_buffer_t * line_buffer);