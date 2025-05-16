$(TEST_DIR)/%: $(TEST_FILES_DIR)/%
	mkdir -p $(@D)
	ln -s $< $@

VALGRIND=valgrind --error-exitcode=42 --show-leak-kinds=all --leak-check=full --errors-for-leak-kinds=all
GDB=gdb --args