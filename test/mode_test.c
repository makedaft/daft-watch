#include <assert.h>
#include <avr/io.h>

#include "button.h"
#include "mode.h"
#include "rtc.h"
#include "stubbed/testutils.h"

void before_each() {
  button_inactive_for_ticks = 0;
  current_digit_index = 0;
  new_minutes = 0;
  new_hours = 0;
  current_mode = ModeDisplayTime;
}

void after_each() {}

void test_switch_to_set_time_mode() {
  current_mode = ModeDisplayTime;
  apply_button_state(ButtonLongPressed);
  assert(current_mode == ModeSetTime);
}

void test_switch_back_to_display_after_timeout_on_idle() {
  current_mode = ModeSetTime;
  apply_button_state(ButtonActive); // Start on active

  for (int i = 0; i < SET_MODE_TIMEOUT - 1; i++) {
    apply_button_state(ButtonIdle);
    assert(current_mode == ModeSetTime);
  }
  apply_button_state(ButtonIdle);

  // Go back to display mode after timeout
  assert(current_mode == ModeDisplayTime);
}

void test_extend_timeout_on_button_activity() {
  current_mode = ModeSetTime;
  apply_button_state(ButtonActive); // Start on active

  for (int i = 0; i < SET_MODE_TIMEOUT - 1; i++) {
    apply_button_state(ButtonIdle);
    assert(current_mode == ModeSetTime);
  }
  apply_button_state(ButtonActive);
  apply_button_state(ButtonIdle);

  // Stay on set mode if interrupted with a button active state
  assert(current_mode == ModeSetTime);
}

void test_update_time() {
  current_mode = ModeSetTime;
  current_digit_index = 0;
  new_hours = 12;
  new_minutes = 34;
  hours = 0;
  minutes = 0;

  apply_button_state(ButtonPressed);
  assert(new_hours == 22);
  assert(new_minutes == 34);

  apply_button_state(ButtonLongPressed); // Next digit
  apply_button_state(ButtonPressed);
  assert(new_hours == 23);
  assert(new_minutes == 34);

  apply_button_state(ButtonLongPressed); // Next digit
  apply_button_state(ButtonPressed);
  assert(new_hours == 23);
  assert(new_minutes == 44);

  apply_button_state(ButtonLongPressed); // Next digit
  apply_button_state(ButtonPressed);
  assert(new_hours == 23);
  assert(new_minutes == 45);

  assert(hours == 0);
  assert(minutes == 0);
  apply_button_state(ButtonLongPressed); // Apply new time to rtc
  assert(hours == 23);
  assert(minutes == 45);
}

int main() {
  TEST test_switch_to_set_time_mode();
  TEST test_switch_back_to_display_after_timeout_on_idle();
  TEST test_extend_timeout_on_button_activity();
  TEST test_update_time();
  return 0;
}

