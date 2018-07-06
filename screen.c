
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#define PIXEL_COUNT_WIDTH 64
#define PIXEL_COUNT_HEIGHT 32

#define PIXEL_WIDTH 20
#define PIXEL_HEIGHT 20
#define WINDOW_WIDTH PIXEL_COUNT_WIDTH * PIXEL_WIDTH
#define WINDOW_HEIGHT PIXEL_COUNT_HEIGHT * PIXEL_HEIGHT


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool pixels[PIXEL_COUNT_WIDTH][PIXEL_COUNT_HEIGHT];

void init_display(void) {
    window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                              WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void update_display(void) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect pixel;

    for (int x = 0; x < PIXEL_COUNT_WIDTH; x++) {
        for (int y = 0; y < PIXEL_COUNT_HEIGHT; y++) {
            if (pixels[x][y] == true) {
                pixel.x = x * PIXEL_WIDTH;
                pixel.y = y * PIXEL_HEIGHT;
                pixel.w = PIXEL_WIDTH;
                pixel.h = PIXEL_HEIGHT;

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);

    SDL_Delay(50);
}

void clear_display(void) {
    memset(pixels, 0, sizeof(pixels));
    update_display();
}

uint8_t draw_sprite(uint8_t x, uint8_t y, int n, uint8_t *addr) {
    uint8_t is_collision = 0;
    uint8_t orig_x = x;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 8; j++) {
            if (((addr[i] >> (7 - j)) & 1) == 0) {
                x = (x + 1) % PIXEL_COUNT_WIDTH;
                continue;
            }
            if (pixels[x][y] == true) {
                is_collision = 1;
            }
            pixels[x][y] ^= true;
            x = (x + 1) % PIXEL_COUNT_WIDTH;
        }
        x = orig_x;
        y = (y + 1) % PIXEL_COUNT_HEIGHT;
    }

    update_display();

    return is_collision;
}
