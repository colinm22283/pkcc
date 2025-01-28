#pragma once

#include <stdio.h>

#include <scanner/token_buffer.h>

typedef struct {
    token_buffer_t tb;
} scanner_t;

void scanner_init(scanner_t * scanner);
void scanner_free(scanner_t * scanner);

void scanner_scan(scanner_t * scanner, const char * buffer);