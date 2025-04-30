include $(TEST_INCLUDE)

ARGUMENTS=-d -3 -fdump-tree -I $(TEST_DIR) $(TEST_DIR)/test.c -o $(TEST_DIR)/test.txt
COMMAND=$(PKCC) $(ARGUMENTS)

FILES=test.c

FILES_EXPANDED=$(foreach f, $(FILES), $(TEST_DIR)/$f)

.PHONY: run
run: $(FILES_EXPANDED)
	cd $(TEST_DIR) && $(COMMAND)

.PHONY: gdb
gdb: $(FILES_EXPANDED)
	cd $(TEST_DIR) && $(GDB) $(COMMAND)

.PHONY: valgrind
valgrind: $(FILES_EXPANDED)
	cd $(TEST_DIR) && $(VALGRIND) $(COMMAND)