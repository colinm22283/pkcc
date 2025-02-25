#pragma once

#include <line_buffer.h>
#include <scanner/token.h>
#include <scanner/token_buffer.h>

size_t scanner_parse_number(
    token_buffer_t * tbuf,
    file_name_entry_t * file_name,
    line_buffer_t * line_buffer,
    size_t line_number,
    size_t position,
    token_t ** token
);