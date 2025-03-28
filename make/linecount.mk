PATTERNS=*.c *.h Makefile *.mk *.sh

FILES=$(foreach p, $(PATTERNS), $(shell find . -type f -name '$p'))

.PHONY: linecount
linecount:
	wc -l $(FILES)