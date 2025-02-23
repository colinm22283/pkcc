TEST_DIRS=$(sort $(dir $(wildcard */*)))

TEST_TARGETS=$(foreach t, $(TEST_DIRS:/=), test-$t)

.PHONY: test-%
test-%:
	cd $* && PKCC=$(PKCC) TEST_DIR=$(TEST_BUILD_DIR)/$* TEST_FILES_DIR=$(CURDIR)/$*/files TEST_INCLUDE=$(CURDIR)/test.mk $(MAKE)

.PHONY: tests
tests: $(TEST_TARGETS)