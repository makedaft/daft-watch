#ifndef _LOADED_DISPLAY
#define _LOADED_DISPLAY
#include <stdint.h>

// "_gfedcba"
// Reversed because a-g is mapped to pa0-6. (pa7 is decimal)
// Complemented because display is common anode
static const uint32_t digit_masks[] = {
    0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001,
    0b0010010, 0b0000010, 0b1111000, 0b0000000, 0b0010000,
};

// Current digit position
volatile extern uint8_t current_digit;

void display_setup(void);

void write_time(uint8_t digit_index);

void display_render(void);

#endif
