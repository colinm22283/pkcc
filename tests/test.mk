$(TEST_DIR)/%: $(TEST_FILES_DIR)/%
	mkdir -p $(@D)
	cp $< $@

VALGRIND=valgrind --error-exitcode=42 --show-leak-kinds=all --leak-check=full --errors-for-leak-kinds=all