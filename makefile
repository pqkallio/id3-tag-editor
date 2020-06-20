IDIR = ./src
CC = gcc
_DEPS = parser/parser.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

src = $(wildcard src/*.c) $(wildcard src/**/*.c)

test = $(wildcard test/*.c)
all_test = $(test) $(filter-out src/main.c, $(src))

obj = $(src:.c=.o)
testo = $(test:.c=.o)
all_testo = $(all_test:.c=.o)

CCFLAGS = -Wall -Wsign-compare -Wextra -Wpedantic -g -I$(IDIR)
TESTFLAGS = -lcunit -I.
LDFLAGS = -lncurses -lpanel

src/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS) $(LDFLAGS)

test/%.o: test/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS) $(TESTFLAGS) $(LDFLAGS)

mp3-header-editor: $(obj) $(DEPS)
	$(CC) -o $@ $^ $(CCFLAGS) $(LDFLAGS)

tester: $(all_testo) $(DEPS)
	$(CC) -o $@ $^ $(CCFLAGS) $(TESTFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) $(testo) mp3-header-editor tester
