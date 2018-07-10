
main: main.c
	gcc main.c core.c screen.c timers.c -Wall -Wextra -lSDL2 -o chip8
