
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "core.h"
#include "screen.h"
#include "timers.h"

// 2 millisecond per tick is 500 Hz
#define MS_PER_TICK 2

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
    Uint32 start_tick_time;
    Uint32 elapsed_tick_time;
    while (!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        start_tick_time = SDL_GetTicks();
        exec_op();
        process_delay_timer();
        process_sound_timer();
        elapsed_tick_time = SDL_GetTicks() - start_tick_time;
        if (elapsed_tick_time < MS_PER_TICK) {
            SDL_Delay(MS_PER_TICK - elapsed_tick_time);
        }
    }
}
