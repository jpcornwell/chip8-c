
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "core.h"
#include "screen.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Wrong number of arguments\n");
        exit(1);
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    load_rom(argv[1]);
    init_display();
    while (1) {
        exec_op();
    }
}
