CC = gcc
CFLAGS = -Wall -Wextra -O0
LDFLAGS = -lm



all: main

main: main.o ./src/body.o ./src/system.o
	gcc $(CFLAGS) main.c ./src/body.c ./src/system.c -o main $(LDFLAGS)



clean:
	rm main.o ./src/body.o ./src/system.o