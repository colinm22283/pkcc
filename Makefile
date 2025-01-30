LDFLAGS+=-lc

SOURCE_DIR=src
MAKE_DIR=make
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/obj

INCLUDE_DIRS=include

export OBJS=
include $(MAKE_DIR)/objs.mk
include $(MAKE_DIR)/targets.mk

.DEFAULT: all
.PHONY: all
all: $(BUILD_DIR)/pkcc

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

.PHONY: test
test: all $(BUILD_DIR)/test.c
	cd build && ./pkcc -dd test.c -o test.s