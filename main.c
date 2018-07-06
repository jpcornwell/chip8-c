
#include <stdbool.h>
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

    init_core();
    init_display();
    load_rom(argv[1]);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        exec_op();
    }
}
