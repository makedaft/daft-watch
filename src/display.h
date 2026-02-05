#ifndef _LOADED_DISPLAY
#define _LOADED_DISPLAY
#include <stdint.h>

void setup_display();

void write_digit(uint8_t digit);

void write_time(uint16_t hour, uint16_t minute, uint8_t digit_mask);

#endif
