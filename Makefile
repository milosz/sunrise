CC=gcc
CFLAGS=-lm -Wall
OBJ= main.o sun.o julian.o

sun: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
