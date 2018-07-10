
#include <stdint.h>

#include <SDL2/SDL.h>

// 16 milliseconds per tick means 60 Hz
#define TIMER_MS_PER_TICK 16

uint8_t delay_reg;
uint8_t sound_reg;
Uint32 delay_last_tick_time;
Uint32 sound_last_tick_time;

uint8_t load_delay_timer(void) {
    return delay_reg;
}

void set_delay_timer(uint8_t val) {
    delay_reg = val;
    delay_last_tick_time = SDL_GetTicks();
}

void set_sound_timer(uint8_t val) {
    sound_reg = val;
    sound_last_tick_time = SDL_GetTicks();
}

void process_sound_timer(void) {
    if (sound_reg != 0) {
        if (SDL_GetTicks() - sound_last_tick_time > TIMER_MS_PER_TICK) {
            sound_reg -= 1;
            sound_last_tick_time = SDL_GetTicks();
        }
    }
}

void process_delay_timer(void) {
    if (delay_reg != 0) {
        if (SDL_GetTicks() - delay_last_tick_time > TIMER_MS_PER_TICK) {
            delay_reg -= 1;
            delay_last_tick_time = SDL_GetTicks();
        }
    }
}
