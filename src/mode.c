#include <stdint.h>

#include "mode.h"
#include "rtc.h"

enum Mode current_mode = ModeDisplayTime;
volatile uint8_t new_minutes = 0;
volatile uint8_t new_hours = 0;
volatile uint8_t current_digit_index = 0;
volatile uint16_t time_set_timeout_ticks = 0;

void increment_digit() {
  switch (current_digit_index) {
  case 0:
    new_hours = (new_hours + 10) % 24;
    break;
  case 1:
    new_hours = (new_hours + 1) % 24;
    break;
  case 2:
    new_minutes = (new_minutes + 10) % 60;
    break;
  case 3:
    new_minutes = (new_minutes + 1) % 60;
    break;
  }
}

void apply_button_state(enum ButtonState button_state) {
  switch (current_mode) {
  case ModeDisplayTime:
    switch (button_state) {
    case ButtonLongPressed:
      new_minutes = minutes;
      new_hours = hours;
      current_digit_index = 0;
      current_mode = ModeSetTime;
      break;
    default:
      break;
    }
    break;
  case ModeSetTime:
    if (button_state == ButtonIdle) {
      time_set_timeout_ticks =
          time_set_timeout_ticks == 0 ? 0 : time_set_timeout_ticks - 1;
      if (time_set_timeout_ticks == 0) {
        current_mode = ModeDisplayTime;
        return;
      }
    } else {
      time_set_timeout_ticks = 10 * 1024;
    }

    switch (button_state) {
    case ButtonPressed:
      increment_digit();
      break;
    case ButtonLongPressed:
      if (current_digit_index == 3) {
        rtc_set_minutes(new_minutes);
        rtc_set_hours(new_hours);
        current_mode = ModeDisplayTime;
        current_digit_index = 0;
      } else {
        current_digit_index++;
      }
      break;
    default:
      break;
    }
    break;
  }
}
