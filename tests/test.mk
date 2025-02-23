$(TEST_DIR)/%: $(TEST_FILES_DIR)/%
	mkdir -p $(@D)
	cp $< $@