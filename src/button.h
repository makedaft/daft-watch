#ifndef _LOADED_BUTTON
#define _LOADED_BUTTON
#include <stdbool.h>

#include "mode.h"

#define DEBOUNCE_TICK_COUNT 10
#define LONG_PRESS_TICK_COUNT 60
#define MAX_TICK_COUNT 80
#define BUTTON_INACTIVE_TICK_COUNT 10

bool extern is_button_active;

void button_tick();

enum ButtonState check_button_state();

// Exposed for test
uint16_t extern button_tick_count;
uint16_t extern button_inactive_for_ticks;

#endif
