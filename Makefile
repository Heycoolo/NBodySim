CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -O0 -g
LDFLAGS = -lm



all: main

main: main.o ./src/body.o ./src/system.o
	gcc $(CFLAGS) main.c ./src/body.c ./src/system.c -o main $(LDFLAGS)



clean:
	rm main.o ./src/body.o ./src/system.o