CC = gcc
CFLAGS = -std=c99
LDFLAGS = -lm

.PHONY: all clean

all: md2

md2: md2.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f md2 *.o a.out