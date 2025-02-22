#include <parsing/escape_characters.h>

char parse_escape_char(const char * str, size_t * read_size) {
    switch (str[0]) {
        case 'a': *read_size = 1; return '\a';
        case 'b': *read_size = 1; return '\b';
        case 'f': *read_size = 1; return '\f';
        case 'n': *read_size = 1; return '\n';
        case 'r': *read_size = 1; return '\r';
        case 't': *read_size = 1; return '\t';
        case 'v': *read_size = 1; return '\v';
        case '\\': *read_size = 1; return '\\';
        case '\'': *read_size = 1; return '\'';
        case '\"': *read_size = 1; return '\"';
        case '?': *read_size = 1; return '\?';

        case 'x': {
            // TODO: implement hex chars
        } break;

        case 'u': {
            // TODO: implement unicode
        } break;
        case 'U': {
            // TODO: implement extended unicode
        } break;

        case '\0': *read_size = 0; break;
    }

    return '\0';
}