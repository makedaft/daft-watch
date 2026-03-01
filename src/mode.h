#ifndef _LOADED_MODE
#define _LOADED_MODE
#include <stdint.h>
enum Mode {
  ModeDisplayTime,
  ModeSetTime,
};

#define SET_MODE_TIMEOUT 1000

enum ButtonState {
  ButtonIdle = 0,
  ButtonActive = 1,
  ButtonPressed = 2,
  ButtonLongPressed = 3,
};

extern enum Mode current_mode;
extern volatile uint8_t new_minutes;
extern volatile uint8_t new_hours;

void apply_button_state(enum ButtonState button_state);

// Just for tests
extern volatile uint8_t current_digit_index;
extern volatile uint16_t time_set_timeout_ticks;

#endif
