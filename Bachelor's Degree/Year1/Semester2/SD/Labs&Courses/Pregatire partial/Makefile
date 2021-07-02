.phony: build clean

SRC = $(foreach src,$(wildcard *c),$(patsubst test%,,$(src)))
EXEC = $(patsubst %.c,%,$(SRC))
HEADERS = $(wildcard *.h)

CC = gcc
CC_FLAGS = -std=c9x -g -O0

build: $(EXEC) $(TEST)

$(EXEC):%:%.c $(HEADERS)
	$(CC) $(CC_FLAGS) $(firstword $+) -o $@

$(TEST):%:%.c $(HEADERS)
	$(CC) $(CC_FLAGS) $(firstword $+) -o $@

clean:
	rm -f $(EXEC) $(TEST)
