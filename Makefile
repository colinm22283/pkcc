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

.PHONY: test
test: all
	echo "int main() {" > build/test.c
	echo "int a = 3;" >> build/test.c
	echo "return a;" >> build/test.c
	echo "}" >> build/test.c

	cd build && ./pkcc -dd test.c