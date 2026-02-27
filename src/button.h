#ifndef _LOADED_BUTTON
#define _LOADED_BUTTON
#include <stdbool.h>
#include <stdint.h>

#define DEBOUNCE_TICK_COUNT 10
#define LONG_PRESS_TICK_COUNT 60
#define MAX_TICK_COUNT 80

enum ButtonState {
  BUTTON_IDLE = 0,
  BUTTON_PRESSED = 1,
  BUTTON_LONG_PRESSED = 2,
};

uint16_t extern button_tick_count;

void button_tick();
#endif
