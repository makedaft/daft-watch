#include <avr/io.h>
#include <stdint.h>

#include "button.h"
#include "mode.h"

void button_tick() { apply_button_state(check_button_state()); }

bool is_button_active = false;
uint16_t button_tick_count = 0;
uint16_t button_inactive_for_ticks = 0;

enum ButtonState check_button_state() {
  // Disable button for some time
  if (button_inactive_for_ticks > 0) {
    button_inactive_for_ticks--;
    return ButtonIdle;
  }

  bool was_active = is_button_active;
  bool in_debounce_period =
      button_tick_count > 0 && button_tick_count < DEBOUNCE_TICK_COUNT;

  // Ignore readings from debounce period
  if (!is_button_active || !in_debounce_period) {
    DDRA &= ~(1 << PA0);
    PORTA &= ~(1 << PA0);
    is_button_active = (PINA & (1 << PA0)) == 0;
  }

  if (is_button_active) {
    button_tick_count++;
    if (button_tick_count >= LONG_PRESS_TICK_COUNT) {
      button_tick_count = 0;
      is_button_active = false;
      button_inactive_for_ticks = BUTTON_INACTIVE_TICK_COUNT;
      return ButtonLongPressed;
    }
    return ButtonActive;
  } else {
    if (button_tick_count >= DEBOUNCE_TICK_COUNT) {
      button_tick_count = 0;
      button_inactive_for_ticks = BUTTON_INACTIVE_TICK_COUNT;
      return ButtonPressed;
    }
    if (!was_active)
      button_tick_count = 0;
  }

  return ButtonIdle;
}
