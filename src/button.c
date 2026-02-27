#include <avr/io.h>
#include <stdint.h>

#include "button.h"
#include "rtc.h"

enum ButtonState check_button_state();

void button_tick() {
  enum ButtonState button_state = check_button_state();
  switch (button_state) {
  case BUTTON_PRESSED:
    if (seconds <= 10)
      seconds = 0;
    else
      seconds -= 10;
    break;
  case BUTTON_LONG_PRESSED:
    seconds += 10;
    break;
  default:
    break;
  }
}

bool is_button_active = false;
uint16_t button_tick_count = 0;

enum ButtonState check_button_state() {
  if (is_button_active && button_tick_count >= LONG_PRESS_TICK_COUNT) {
    button_tick_count = 0;
    is_button_active = false;
    return BUTTON_LONG_PRESSED;
  }

  bool in_debounce_period = is_button_active && button_tick_count > 0 &&
                            button_tick_count < DEBOUNCE_TICK_COUNT;

  // Ignore readings from debounce period
  if (!in_debounce_period) {
    DDRA &= ~(1 << PA0);
    PORTA &= ~(1 << PA0);
    is_button_active = (PINA & (1 << PA0)) == 0;
  }

  if (is_button_active) {
    button_tick_count++;
  } else {
    if (button_tick_count >= DEBOUNCE_TICK_COUNT) {
      button_tick_count = 0;
      return BUTTON_PRESSED;
    }
  }

  return BUTTON_IDLE;
}
