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

volatile extern uint8_t current_digit;

void display_setup(void);

static inline uint32_t get_digit_mask(uint8_t digit) {
  return digit_masks[digit % 10];
}

static inline uint8_t get_digit(uint16_t hour, uint16_t minute,
                                uint8_t digit_index) {
  // TODO: use hour
  // uint8_t hour_digit1 = hour % 10;
  // uint8_t hour_digit2 = (hour / 10) % 10;
  switch (digit_index) {
  case 0:
    return minute % 10;
  case 1:
    return (minute / 10) % 10;
  case 2:
    return (minute % 10) + 1;
  case 3:
    return ((minute / 10) % 10) + 1;
  }
  return 0;
}

void write_time(uint16_t hour, uint16_t minute, uint8_t digit_index);

void display_render(void);

#endif
