OBJS+=$(OBJ_DIR)/main.o
OBJS+=$(OBJ_DIR)/options.o
OBJS+=$(OBJ_DIR)/escape_characters.o

OBJS+=$(OBJ_DIR)/debug/error_handler.o

OBJS+=$(OBJ_DIR)/scanner/scanner.o
OBJS+=$(OBJ_DIR)/scanner/token.o
OBJS+=$(OBJ_DIR)/scanner/file_loader.o
OBJS+=$(OBJ_DIR)/scanner/token_buffer.o
OBJS+=$(OBJ_DIR)/scanner/token_translation.o
OBJS+=$(OBJ_DIR)/scanner/token_stringify.o