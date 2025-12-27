INCLUDE_PARAMS=$(foreach d, $(INCLUDE_DIRS) $(EXTRA_INCLUDE_DIRS), -I$d)
HEADERS+=$(foreach d, $(INCLUDE_DIRS), $(shell find $d -type f -name '*.h'))

OPTIMIZATION?=3

$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -O$(OPTIMIZATION) $(INCLUDE_PARAMS) -c $< -o $@
