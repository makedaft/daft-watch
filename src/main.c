#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

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

// Display
// timer 0 has lower priority than timer 1, so some ticks could get skipped due
// to interrupt starvation
ISR(TIM0_COMPA_vect, ISR_FLATTEN) { display_render(); }

// RTC interrupt
ISR(TIM1_COMPA_vect, ISR_FLATTEN) { rtc_increment(); }

#define IS_BUTTON_ON() ((PINA & (1 << PA0)) == 0)
#define ENABLE_BUTTON() (DDRA &= ~(1 << PA0))

int main(void) {
  PRR = 0b0011; // Power reduction register. Shut down USI and ADC

  display_setup();
  rtc_setup();

  sei();

  while (1) {
    // ENABLE_BUTTON();
    // if (IS_BUTTON_ON()) {
    //   cli();
    //   ENABLE_BUTTON();
    //   _delay_ms(25); // Debounced
    //   if (IS_BUTTON_ON()) {
    //     seconds += 1;
    //     sei();
    //     _delay_ms(50);
    //     continue;
    //   }
    // }
    // sei();

#ifdef STUBBED
    stub_run_timers();
    _delay_ms(100);
#endif
  }

  return 0;
}
