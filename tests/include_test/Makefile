include $(TEST_INCLUDE)

ARGUMENTS=-dd -E -I $(TEST_DIR) $(TEST_DIR)/test.c -o $(TEST_DIR)/test.pp
COMMAND=$(PKCC) $(ARGUMENTS)

FILES=test.c test.h

FILES_EXPANDED=$(foreach f, $(FILES), $(TEST_DIR)/$f)

.PHONY: run
run: $(FILES_EXPANDED)
	$(COMMAND)

.PHONY: valgrind
valgrind: $(FILES_EXPANDED)
	$(VALGRIND) $(COMMAND)