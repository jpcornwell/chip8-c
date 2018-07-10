
#ifndef TIMERS_H
#define TIMERS_H

void set_sound_timer(uint8_t);
void set_delay_timer(uint8_t);
uint8_t load_delay_timer(void);
void process_sound_timer(void);
void process_delay_timer(void);

#endif
