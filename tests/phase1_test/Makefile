include $(TEST_INCLUDE)

.PHONY: run
run: $(TEST_DIR)/test.c
	$(PKCC) -dd -1 $(TEST_DIR)/test.c -o $(TEST_DIR)/test.pp

.PHONY: valgrind
valgrind: $(TEST_DIR)/test.c
	$(VALGRIND) $(PKCC) -dd -1 $(TEST_DIR)/test.c -o $(TEST_DIR)/test.pp