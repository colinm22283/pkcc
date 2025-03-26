include $(TEST_INCLUDE)

ARGUMENTS=-d -2 -I $(TEST_DIR) $(TEST_DIR)/test.c -o $(TEST_DIR)/test.pp
COMMAND=$(PKCC) $(ARGUMENTS)

FILES=test.c

FILES_EXPANDED=$(foreach f, $(FILES), $(TEST_DIR)/$f)

.PHONY: run
run: $(FILES_EXPANDED)
	$(COMMAND)

.PHONY: gdb
gdb: $(FILES_EXPANDED)
	$(GDB) $(COMMAND)

.PHONY: valgrind
valgrind: $(FILES_EXPANDED)
	$(VALGRIND) $(COMMAND)