
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#define PIXEL_COUNT_WIDTH 64
#define PIXEL_COUNT_HEIGHT 32

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

SDL_Window *window = NULL;
SDL_Surface *screen_surface = NULL;

bool pixels[PIXEL_COUNT_WIDTH][PIXEL_COUNT_HEIGHT];

void init_display(void) {
    window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                              WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    screen_surface = SDL_GetWindowSurface(window);
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format,
                                                  0xff, 0xff, 0xff));
    SDL_UpdateWindowSurface(window);

    SDL_Delay(2000);
}

void clear_display(void) {
    memset(pixels, 0, sizeof(pixels));
    printf("Clear display stub\n");
}

uint8_t draw_sprite(uint8_t x, uint8_t y, int n, uint8_t *addr) {
    uint8_t is_collision = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 8; j++) {
            if (((addr[i] >> j) & 1) == 0) {
                x = (x + 1) % PIXEL_COUNT_WIDTH;
                continue;
            }
            if (pixels[x][y] == true) {
                is_collision = 1;
            }
            pixels[x][y] ^= true;
            x = (x + 1) % PIXEL_COUNT_WIDTH;
        }
        x = (x - 8) % PIXEL_COUNT_WIDTH;
        y = (y + 1) % PIXEL_COUNT_HEIGHT;
    }

    printf("Draw to screen stub\n");

    return is_collision;
}
