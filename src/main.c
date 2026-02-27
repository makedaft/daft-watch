#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "button.h"
#include "display.h"
#include "rtc.h"

// Config for attiny84a + external 32KHz crystal osc taken from
// https://github.com/ingepnet/breadboardwatch/blob/master/Makefile#L26
#include <avr/fuse.h>
FUSES = {
    .low = 0xe6,
    .high = 0xd7,
    .extended = 0xff,
};

// Display interrupt
// Timer 0 has lower priority than timer 1, so some ticks could get skipped on
// interrupt starvation
ISR(TIM0_COMPA_vect, ISR_FLATTEN) {
  button_tick(); // Check button click
  display_render();
}

// RTC interrupt
ISR(TIM1_COMPA_vect, ISR_FLATTEN) { rtc_increment(); }

// Button interrupt
ISR(PCINT0_vect, ISR_FLATTEN) { button_tick(); }

int main(void) {
  PRR = 0b0011; // Power reduction register. Shut down USI and ADC

  display_setup();
  rtc_setup();

  sei();

  while (1) {
    // switch (button_tick()) {
    // case BUTTON_PRESSED:
    //   seconds += 5;
    //   break;
    // case BUTTON_LONG_PRESSED:
    //   seconds += 10;
    //   break;
    // default:
    //   break;
    // }
#ifdef STUBBED
    stub_run_timers();
    _delay_ms(100);
#endif
  }

  return 0;
}
