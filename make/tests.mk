export TEST_BUILD_DIR=$(CURDIR)/$(BUILD_DIR)/tests
export PKCC=$(CURDIR)/$(BUILD_DIR)/pkcc

.PHONY: test-%
test-%: $(BUILD_DIR)/pkcc
	cd $(TESTS_DIR) && $(MAKE) test-$*

.PHONY: valgrind-%
valgrind-%: $(BUILD_DIR)/pkcc
	cd $(TESTS_DIR) && $(MAKE) valgrind-$*

.PHONY: tests
tests: $(BUILD_DIR)/pkcc
	cd $(TESTS_DIR) && $(MAKE) tests