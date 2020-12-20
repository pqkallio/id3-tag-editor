IDIR = ./src
COVDIR = coverage
CC = gcc
COV = lcov
HTMLGEN = genhtml
_DEPS =	dstructs/hashset.h \
				mem/mem.h \
				parser/parser.h \
				util/binary.h \
				util/strings.h \
				util/hash.h \
				domain/tagv2.h \
				dstructs/linkedlist.h \
				dstructs/hashmap.h \
				dstructs/stack.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
EXEC = id3-tag-editor
TESTER = tester

src = $(wildcard src/*.c) $(wildcard src/**/*.c)

test = $(wildcard test/*.c)
all_test = $(test) $(filter-out src/main.c, $(src))

obj = $(src:.c=.o)
testo = $(test:.c=.o)
all_testo = $(all_test:.c=.o)
coverage_files = $(wildcard *.gcda) $(wildcard *.gcno) \
									$(wildcard src/*.gcda) $(wildcard src/*.gcno) $(wildcard src/**/*.gcda) $(wildcard src/**/*.gcno) \
									$(wildcard test/*.gcda) $(wildcard test/*.gcno) $(wildcard test/**/*.gcda) $(wildcard test/**/*.gcno)
cov_info = coverage.info
html_cov_path = $(COVDIR)/html

CFLAGS = -Wall -Wsign-compare -Wextra -Wpedantic -g -I$(IDIR) --coverage
TESTFLAGS = -lcunit -I.
LDFLAGS = -lm -lncurses -lpanel --coverage
COVFLAGS = --capture --directory . --output-file $(COVDIR)/$(cov_info)
HTMLGENFLAGS = $(COVDIR)/$(cov_info) --output-directory $(html_cov_path)

src/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

test/%.o: test/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(TESTFLAGS) $(LDFLAGS)

$(EXEC): $(obj) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

tester: $(all_testo) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(TESTFLAGS) $(LDFLAGS)

.PHONY: clean coverage

clean:
	rm -f $(obj) $(testo) $(EXEC) $(TESTER) $(coverage_files)

coverage: tester
	./$(TESTER)
	$(COV) $(COVFLAGS)
	$(HTMLGEN) $(HTMLGENFLAGS)
