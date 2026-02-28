#include "button.h"
#include "mode.h"
#include <assert.h>
#include <avr/io.h>

void test_button_press() {
  assert(check_button_state() == ButtonIdle);

  PINA &= ~(1 << PA0); // Button is pressed
  assert(check_button_state() == ButtonActive);
  for (int i = 0; i < DEBOUNCE_TICK_COUNT; i++)
    assert(check_button_state() == ButtonActive);

  PINA |= (1 << PA0); // Button is released

  assert(check_button_state() == ButtonPressed); // Detects button press
  assert(check_button_state() == ButtonIdle);    // Resets to idle
}

void test_button_long_press() {
  assert(check_button_state() == ButtonIdle);

  // Button is pressed for a while
  PINA &= ~(1 << PA0);
  for (int i = 0; i < LONG_PRESS_TICK_COUNT - 1; i++)
    assert(check_button_state() == ButtonActive);

  assert(check_button_state() == ButtonLongPressed); // Detects button press

  assert(check_button_state() == ButtonIdle); // Resets to idle
}

void test_button_inactive_after_press() {
  assert(check_button_state() == ButtonIdle);

  // Button is pressed
  PINA &= ~(1 << PA0);
  for (int i = 0; i < DEBOUNCE_TICK_COUNT; i++)
    assert(check_button_state() == ButtonActive);
  PINA |= (1 << PA0); // Button is released
  assert(check_button_state() == ButtonPressed);
  assert(check_button_state() == ButtonIdle); // Resets to idle

  // Button is pressed again
  PINA &= ~(1 << PA0);

  // Button stays inactive for some time
  for (int i = 0; i < BUTTON_INACTIVE_TICK_COUNT - 1; i++)
    assert(check_button_state() == ButtonIdle);
  // Button becomes active again after delay
  assert(check_button_state() == ButtonActive);
}

void before_each() {
  PINA = (1 << PA0); // Active on low
  button_tick_count = 0;
  button_inactive_for_ticks = 0;
}
#define TEST before_each();

int main() {
  TEST test_button_press();
  TEST test_button_long_press();
  TEST test_button_inactive_after_press();
  return 0;
}

