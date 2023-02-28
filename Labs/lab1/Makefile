.phony: build clean

SRC = $(wildcard *.c)
EXEC = $(patsubst %.c,%,$(SRC))
OTHER = OrderedSet.h

CC = gcc
CC_FLAGS = -std=c9x

build: $(EXEC)

$(EXEC):%:%.c $(OTHER)
	$(CC) $(CC_FLAGS) $(firstword $+) -o $@

clean:
	rm -f $(EXEC)
