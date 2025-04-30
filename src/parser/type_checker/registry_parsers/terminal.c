#include <parser/type_checker/error.h>
#include <parser/type_checker/registry_parsers/terminal.h>

#include <alloc.h>

type_checker_type_t * type_checker_registry_parse_terminal(
    type_checker_registry_t * tr,
    token_buffer_t * token_buffer,
    syntax_tree_node_list_node_t * terminal_node
) {
    if (terminal_node->token_type != RT_TERMINAL) type_checker_error();

    switch (terminal_node->terminal.terminal) {
        case token_number_constant(): {
            switch (token_buffer->tokens[terminal_node->terminal.position].constant_data->type) {
                case SCANNER_CONSTANT_TYPE_UC: return uchar_type;
                case SCANNER_CONSTANT_TYPE_US: return ushort_type;
                case SCANNER_CONSTANT_TYPE_UI: return uint_type;
                case SCANNER_CONSTANT_TYPE_UL: return ulong_type;
                case SCANNER_CONSTANT_TYPE_ULL: return ulonglong_type;

                case SCANNER_CONSTANT_TYPE_SC: return schar_type;
                case SCANNER_CONSTANT_TYPE_SS: return sshort_type;
                case SCANNER_CONSTANT_TYPE_SI: return sint_type;
                case SCANNER_CONSTANT_TYPE_SL: return slong_type;
                case SCANNER_CONSTANT_TYPE_SLL: return slonglong_type;

                case SCANNER_CONSTANT_TYPE_FLOAT: return float_type;
                case SCANNER_CONSTANT_TYPE_DOUBLE: return double_type;
            }
        } break;
    }

    return NULL;
}