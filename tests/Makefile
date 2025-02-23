TEST_DIRS=$(sort $(dir $(wildcard */*)))

TEST_TARGETS=$(foreach t, $(TEST_DIRS:/=), test-$t)
VALGRIND_TARGETS=$(foreach t, $(TEST_DIRS:/=), valgrind-$t)

.PHONY: test-%
test-%:
	cd $* && PKCC=$(PKCC) TEST_DIR=$(TEST_BUILD_DIR)/$* TEST_FILES_DIR=$(CURDIR)/$*/files TEST_INCLUDE=$(CURDIR)/test.mk $(MAKE) run

.PHONY: valgrind-%
valgrind-%:
	cd $* && PKCC=$(PKCC) TEST_DIR=$(TEST_BUILD_DIR)/$* TEST_FILES_DIR=$(CURDIR)/$*/files TEST_INCLUDE=$(CURDIR)/test.mk $(MAKE) valgrind

.PHONY: tests
tests: $(TEST_TARGETS) $(VALGRIND_TARGETS)