include $(TEST_INCLUDE)

ARGUMENTS=-4 -fdump-tree -I $(TEST_DIR) $(TEST_DIR)/test.c -o $(TEST_DIR)/main.j
COMMAND=$(PKCC) $(ARGUMENTS)

FILES=test.c lib440.java

FILES_EXPANDED=$(foreach f, $(FILES), $(TEST_DIR)/$f)

.PHONY: run
run: $(FILES_EXPANDED)
	cd $(TEST_DIR) && $(COMMAND)
	~/Krakatau/target/release/krak2 asm --out $(TEST_DIR)/main.class $(TEST_DIR)/main.j
	cd $(TEST_DIR) && javac -d . lib440.java
	cd $(TEST_DIR) && java main lib440 ; echo "Exited with code $$?"

.PHONY: gdb
gdb: $(FILES_EXPANDED)
	cd $(TEST_DIR) && $(GDB) $(COMMAND)

.PHONY: valgrind
valgrind: $(FILES_EXPANDED)
	cd $(TEST_DIR) && $(VALGRIND) $(COMMAND)