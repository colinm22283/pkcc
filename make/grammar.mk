GRAMMAR_OUT_DIR=$(BUILD_DIR)/grammar
GRAMMAR_DIR=grammar

YACC=yacc
LEX=lex

EXTRA_INCLUDE_DIRS+=$(GRAMMAR_OUT_DIR)/include

$(GRAMMAR_OUT_DIR)/include/c99.h: $(GRAMMAR_DIR)/c99.y
	mkdir -p $(@D)

	$(YACC) -d $< -o $@

$(GRAMMAR_OUT_DIR)/src/c99.c: $(GRAMMAR_DIR)/c99.l
	mkdir -p $(@D)

	$(LEX) -o $@ $<