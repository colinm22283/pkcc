#include <stdint.h>
#include <stdlib.h>

#include <parsing/is_number_char.h>
#include <parsing/is_number_postfix_char.h>
#include <parsing/case_insensitive_compare.h>
#include <parsing/is_symbol_char.h>
#include <scanner/parse_number.h>
#include <debug/line_error.h>
#include <debug/log.h>
#include <debug/line_warning.h>

static inline uint8_t digit_to_number(char c) {
    if (c <= '9') return c - '0';
    else return c - 'A' + 10;
}

size_t scanner_parse_number(
    token_buffer_t * tbuf,
    file_name_entry_t * file_name,
    line_buffer_t * line_buffer,
    size_t line_number,
    size_t source_line_number,
    size_t position,
    token_t ** token_out
) {
    size_t start_position = position;

    log_printf("Parsing numerical constant\n");

    line_buffer_line_t * line = &line_buffer->lines[line_number];

    const char * data = line->line;

    bool is_integer = true;

    {
        size_t i = position;

        do if (data[i] == '.') is_integer = false;
        while(is_number_char(data[i++]));
    }

    if (is_integer) log_printf("Number is an integer\n");
    else log_printf("Number is real\n");

    if (is_integer) {
        enum {
            BINARY, OCTAL, DECIMAL, HEXADECIMAL
        } radix;

        if (data[position] == '0') {
            position++;

            if (case_insensitive_compare(data[position], 'b')) {
                radix = BINARY;
                position++;
            }
            else if (case_insensitive_compare(data[position], 'x')) {
                radix = HEXADECIMAL;
                position++;
            }
            else {
                radix = OCTAL;
                position--;
            }
        }
        else radix = DECIMAL;

        switch (radix) {
            case BINARY: log_printf("Number is binary\n"); break;
            case OCTAL: log_printf("Number is octal\n"); break;
            case DECIMAL: log_printf("Number is decimal\n"); break;
            case HEXADECIMAL: log_printf("Number is hexadecimal\n"); break;
        }

        size_t number_length = 0;
        while (
            is_number_char(data[position + number_length]) ||
            (data[position + number_length] >= 'a' && data[position + number_length] <= 'f') ||
            (data[position + number_length] >= 'A' && data[position + number_length] <= 'F')
        ) number_length++;

        log_printf("Number length is %zu\n", number_length);

        unsigned long long accumulator = 0;
        unsigned long long radix_value;
        switch (radix) {
            case BINARY: radix_value = 2; break;
            case OCTAL: radix_value = 8; break;
            case DECIMAL: radix_value = 10; break;
            case HEXADECIMAL: radix_value = 16; break;
        }
        unsigned long long multiplier = 1;
        for (size_t i = 0; i < number_length; i++) {
            size_t pos = position + number_length - i - 1;

            accumulator += multiplier * digit_to_number(data[pos]);

            multiplier *= radix_value;
        }

        log_printf("Read number %llu\n", accumulator);

        position += number_length;

        size_t postfix_length = 0;
        while (is_number_postfix_char(data[position + postfix_length])) postfix_length++;

        size_t l_count = 0, u_count = 0;
        for (size_t i = 0; i < postfix_length; i++) {
            if (case_insensitive_compare(data[position + i], 'u')) u_count++;
            else if (case_insensitive_compare(data[position + i], 'l')) l_count++;
        }

        if (l_count > 2) {
            fatal_line_range_error(
                line_buffer,
                file_name,
                "Invalid amount of long specifiers in number postfix",
                line_number,
                position,
                postfix_length
            );
        }
        if (u_count > 1) {
            fatal_line_range_error(
                line_buffer,
                file_name,
                "Invalid amount of unsigned specifiers in number postfix",
                line_number,
                position,
                postfix_length
            );
        }

        token_t * token = token_buffer_push(tbuf, TOKEN_TYPE_CONSTANT);
        token_data_constant_t * constant = (token_data_constant_t *) token->data;

        token->file_name = line->metadata.file_name;
        token->line = source_line_number;
        token->position = position;

        constant->i = accumulator;

        log_printf("L count = %zu\n", l_count);
        log_printf("U count = %zu\n", u_count);

        if (u_count == 0) {
            switch (l_count) {
                case 0: constant->type = SCANNER_CONSTANT_TYPE_SI; break;
                case 1: constant->type = SCANNER_CONSTANT_TYPE_SL; break;
                case 2: constant->type = SCANNER_CONSTANT_TYPE_SLL; break;

                default: break;
            }
        }
        else {
            switch (l_count) {
                case 0: constant->type = SCANNER_CONSTANT_TYPE_UI; break;
                case 1: constant->type = SCANNER_CONSTANT_TYPE_UL; break;
                case 2: constant->type = SCANNER_CONSTANT_TYPE_ULL; break;

                default: break;
            }
        }

        if (token_out != NULL) *token_out = token;

        position += postfix_length;

        if (options.max_integer_length != 0 && position - start_position > options.max_integer_length) {
            line_range_warning(
                line_buffer,
                file_name,
                "Excess integer length (see -fmax-integer-literal-length)",
                line_number,
                start_position,
                position - start_position
            );
        }

        return position;
    }
    else {
        size_t number_length = 0;
        while (
            is_number_char(data[position + number_length]) ||
            data[position + number_length] == '.' ||
            case_insensitive_compare(data[position + number_length], 'e')
        ) number_length++;

        bool is_double = true;

        if (data[position + number_length] == 'f') is_double = false;
        else if (!case_insensitive_compare(data[position + number_length], 'e') && is_symbol_char(data[position + number_length])) {
            fatal_line_error(
                line_buffer,
                file_name,
                "Invalid real constant postfix",
                line_number,
                position + number_length
            );
        }

        token_t * token = token_buffer_push(tbuf, TOKEN_TYPE_CONSTANT);
        token_data_constant_t * constant = (token_data_constant_t *) token->data;

        token->file_name = line->metadata.file_name;
        token->line = source_line_number;
        token->position = position;

        char * end_ptr;

        if (is_double) {
            constant->type = SCANNER_CONSTANT_TYPE_DOUBLE;
            constant->d = strtod(&data[position], &end_ptr);
        }
        else {
            constant->type = SCANNER_CONSTANT_TYPE_FLOAT;
            constant->f = strtof(&data[position], &end_ptr);
        }

        if (end_ptr == &data[position]) {
            fatal_line_range_error(
                line_buffer,
                file_name,
                "Invalid real constant",
                line_number,
                position,
                number_length
            );
        }

        position += (intptr_t) end_ptr - (intptr_t) &data[position] + (is_double ? 0 : 1);

        if (options.max_real_length != 0 && position - start_position > options.max_real_length) {
            line_range_warning(
                line_buffer,
                file_name,
                "Excess real length (see -fmax-real-constant-length)",
                line_number,
                start_position,
                position - start_position
            );
        }

        return position;
    }
}