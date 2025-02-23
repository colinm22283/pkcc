include $(TEST_INCLUDE)

.PHONY: run
run: $(TEST_DIR)/test.c $(TEST_DIR)/test.h
	$(PKCC) -dd -E -I $(TEST_DIR) $(TEST_DIR)/test.c -o $(TEST_DIR)/test.pp
