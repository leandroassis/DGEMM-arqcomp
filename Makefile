SRC = $(wildcard *c)
OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -Wall -O3 -std=c99

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

dgemm: $(OBJ)
	$(CC) -o $@ $^
