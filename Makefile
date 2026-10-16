CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
RELEASE_FLAGS = -O2 -march=native
TEST_FLAGS = -g

SRCS = demo.c arena.c
OBJS = $(SRCS:%.c=%.o)
OUTPUT = demo
TEST_OUTPUT = tests/test

%.o: %.c
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

.PHONY: all test clean

all: $(OBJS)
	$(CC) $^ -o $(OUTPUT)

$(TEST_OUTPUT): tests/arena_tests.c arena.o
	$(CC) $(CFLAGS) $(TEST_FLAGS) -I. $^ -o $@

test: $(TEST_OUTPUT)
	./$(TEST_OUTPUT)

clean:
	rm -f $(OBJS) $(OUTPUT) $(TEST_OUTPUT)
