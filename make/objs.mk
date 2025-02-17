OBJS+=$(OBJ_DIR)/main.o
OBJS+=$(OBJ_DIR)/options.o
OBJS+=$(OBJ_DIR)/escape_characters.o
OBJS+=$(OBJ_DIR)/file_loader.o
OBJS+=$(OBJ_DIR)/file_writer.o

OBJS+=$(OBJ_DIR)/debug/error_handler.o

OBJS+=$(OBJ_DIR)/preprocessor/preprocessor.o
OBJS+=$(OBJ_DIR)/preprocessor/variable_map.o
OBJS+=$(OBJ_DIR)/preprocessor/comment_filter.o
OBJS+=$(OBJ_DIR)/preprocessor/evaluate.o
OBJS+=$(OBJ_DIR)/preprocessor/preprocessor_token_buffer.o

OBJS+=$(OBJ_DIR)/preprocessor/directives/define.o
OBJS+=$(OBJ_DIR)/preprocessor/directives/undef.o
OBJS+=$(OBJ_DIR)/preprocessor/directives/include.o
OBJS+=$(OBJ_DIR)/preprocessor/directives/if.o
OBJS+=$(OBJ_DIR)/preprocessor/directives/ifdef.o

OBJS+=$(OBJ_DIR)/scanner/scanner.o
OBJS+=$(OBJ_DIR)/scanner/token.o
OBJS+=$(OBJ_DIR)/scanner/token_buffer.o
OBJS+=$(OBJ_DIR)/scanner/token_translation.o
OBJS+=$(OBJ_DIR)/scanner/token_stringify.o