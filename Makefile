SOURCE_DIR=src
MAKE_DIR=make
BUILD_DIR=build
TESTS_DIR=tests
OBJ_DIR=$(BUILD_DIR)/obj

CFLAGS+=-Wall -Wextra

INCLUDE_DIRS=include

.DEFAULT: all
.PHONY: all
all: $(BUILD_DIR)/pkcc

.PHONY: debug
debug: CFLAGS+=-g -fsanitize=address
debug: LDFLAGS+=-fsanitize=address
debug: OPTIMIZATION=0
debug: all

export OBJS=
include $(MAKE_DIR)/objs.mk
include $(MAKE_DIR)/targets.mk
include $(MAKE_DIR)/tests.mk
include $(MAKE_DIR)/linecount.mk

$(BUILD_DIR)/pkcc: $(OBJS)
	mkdir -p $(@D)

	$(CC) $(LDFLAGS) $(OBJS) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: run
run:
	$(MAKE) test-generation_test
	echo "------------ OUTPUT ------------"
	~/temp/Krakatau/target/release/krak2 asm --out build/tests/generation_test/main.class build/tests/generation_test/main.j
	cd build/tests/generation_test && javac -d . lib440.java
	cd build/tests/generation_test && java main lib440

.PHONY: run-gdb
run-gdb:
	$(MAKE) gdb-generation_test
	echo "------------ OUTPUT ------------"
	~/temp/Krakatau/target/release/krak2 asm --out build/tests/generation_test/main.class build/tests/generation_test/main.j
	cd build/tests/generation_test && javac -d . lib440.java
	cd build/tests/generation_test && java main lib440