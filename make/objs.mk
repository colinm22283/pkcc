OBJS+=$(OBJ_DIR)/main.o
OBJS+=$(OBJ_DIR)/options.o
OBJS+=$(OBJ_DIR)/escape_characters.o
OBJS+=$(OBJ_DIR)/line_buffer.o
OBJS+=$(OBJ_DIR)/free_list.o

OBJS+=$(OBJ_DIR)/file/file_exists.o
OBJS+=$(OBJ_DIR)/file/file_loader.o
OBJS+=$(OBJ_DIR)/file/file_writer.o
OBJS+=$(OBJ_DIR)/file/file_name_registry.o

OBJS+=$(OBJ_DIR)/debug/error_handler.o
OBJS+=$(OBJ_DIR)/debug/line_error.o
OBJS+=$(OBJ_DIR)/debug/line_warning.o

OBJS+=$(OBJ_DIR)/preprocessor/preprocessor.o
OBJS+=$(OBJ_DIR)/preprocessor/variable_map.o
OBJS+=$(OBJ_DIR)/preprocessor/comment_filter.o
OBJS+=$(OBJ_DIR)/preprocessor/preprocessor_token_buffer.o

OBJS+=$(OBJ_DIR)/scanner/scanner.o
OBJS+=$(OBJ_DIR)/scanner/token.o
OBJS+=$(OBJ_DIR)/scanner/token_buffer.o
OBJS+=$(OBJ_DIR)/scanner/token_translation.o
OBJS+=$(OBJ_DIR)/scanner/token_stringify.o
OBJS+=$(OBJ_DIR)/scanner/phase1.o
OBJS+=$(OBJ_DIR)/scanner/parse_number.o

OBJS+=$(OBJ_DIR)/parser/parser.o
OBJS+=$(OBJ_DIR)/parser/syntax_tree.o
OBJS+=$(OBJ_DIR)/parser/rule_registry.o
OBJS+=$(OBJ_DIR)/parser/rules.o
OBJS+=$(OBJ_DIR)/parser/phase2.o

OBJS+=$(OBJ_DIR)/parser/type_checker/type_stringify.o
OBJS+=$(OBJ_DIR)/parser/type_checker/type_free.o
OBJS+=$(OBJ_DIR)/parser/type_checker/type_checker.o
OBJS+=$(OBJ_DIR)/parser/type_checker/type_registry.o
OBJS+=$(OBJ_DIR)/parser/type_checker/type_expand.o
OBJS+=$(OBJ_DIR)/parser/type_checker/type_equal.o
OBJS+=$(OBJ_DIR)/parser/type_checker/type_convertable.o
OBJS+=$(OBJ_DIR)/parser/type_checker/type_is_const.o
OBJS+=$(OBJ_DIR)/parser/type_checker/type_verifier.o
OBJS+=$(OBJ_DIR)/parser/type_checker/scope_registry.o
OBJS+=$(OBJ_DIR)/parser/type_checker/variable_registry.o
OBJS+=$(OBJ_DIR)/parser/type_checker/phase3.o
OBJS+=$(OBJ_DIR)/parser/type_checker/registry_parsers/struct.o
OBJS+=$(OBJ_DIR)/parser/type_checker/registry_parsers/type.o
OBJS+=$(OBJ_DIR)/parser/type_checker/registry_parsers/expression.o
OBJS+=$(OBJ_DIR)/parser/type_checker/registry_parsers/terminal.o
OBJS+=$(OBJ_DIR)/parser/type_checker/registry_parsers/decl_var.o
OBJS+=$(OBJ_DIR)/parser/type_checker/registry_parsers/decl_func.o