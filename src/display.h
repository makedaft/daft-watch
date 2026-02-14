#ifndef _LOADED_DISPLAY
#define _LOADED_DISPLAY
#include <stdint.h>

volatile extern uint8_t current_digit;

void display_setup(void);

void write_time(uint16_t hour, uint16_t minute, uint8_t digit_mask);

void display_render(void);

#endif
