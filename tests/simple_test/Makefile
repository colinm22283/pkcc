include $(TEST_INCLUDE)

.PHONY: run
run: $(TEST_DIR)/test.c
	$(PKCC) -dd $(TEST_DIR)/test.c -o $(TEST_DIR)/test.s

.PHONY: valgrind
valgrind: $(TEST_DIR)/test.c
	$(VALGRIND) $(PKCC) -dd $(TEST_DIR)/test.c -o $(TEST_DIR)/test.s
