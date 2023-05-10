SRC = $(wildcard *c)
OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -Wall -O3 -lrt

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

dgemm: $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -rf $(OBJ) ./dgemm

.PHONY: clean
