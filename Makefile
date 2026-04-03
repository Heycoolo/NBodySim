CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -O0
LDFLAGS = -lm -lraylib



all: main

main: main.o ./src/body.o ./src/system.o ./src/runge_kutta.o ./src/anim_sim.o
	gcc $(CFLAGS) main.c ./src/body.c ./src/system.c ./src/runge_kutta.c ./src/anim_sim.c -o main $(LDFLAGS)



clean:
	rm main.o ./src/body.o ./src/system.o ./src/runge_kutta.o ./src/anim_sim.o