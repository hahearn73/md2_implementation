CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

.PHONY: all clean

all: md2

md2: md2.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f md2 *.o a.out
