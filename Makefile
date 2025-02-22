SOURCE_DIR=src
MAKE_DIR=make
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj

CFLAGS+=-Wall -Wextra

INCLUDE_DIRS=include

export OBJS=
include $(MAKE_DIR)/objs.mk
include $(MAKE_DIR)/targets.mk

.DEFAULT: all
.PHONY: all
all: $(BUILD_DIR)/pkcc

.PHONY: debug
debug: CFLAGS+=-g -O0
debug: all

$(BUILD_DIR)/pkcc: $(OBJS)
	mkdir -p $(@D)

	$(CC) $(LDFLAGS) $(OBJS) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/test.c:
	mkdir -p $(@D)

	echo "int main() {" > $@
	echo "    int a = 3;" >> $@
	echo "    static const char * str = \"aaaaa\";" >> $@
	echo "    return a;" >> $@
	echo "}" >> $@


$(BUILD_DIR)/pp_test.c:
	mkdir -p $(@D)

	echo -e "#include \"pp_test.h\"" > $@
	echo "" >> $@
	echo "int main() {" >> $@
	echo "    int a = 3;" >> $@
	echo "    static const char * str = \"aaaaa\";" >> $@
	echo "    return a;" >> $@
	echo "}" >> $@

$(BUILD_DIR)/pp_test.h:
	mkdir -p $(@D)

	echo "int global = 100;" > $@

.PHONY: test_files
test_files: $(BUILD_DIR)/test.c

.PHONY: test
test: all test_files
	cd build && ./pkcc -dd test.c -o test.s

.PHONY: preprocessor_test_files
preprocessor_test_files: $(BUILD_DIR)/pp_test.c $(BUILD_DIR)/pp_test.h

.PHONY: preprocessor_test
preprocessor_test: all preprocessor_test_files
	cd build && ./pkcc -dd -E -I . pp_test.c -o pp_test.pp
