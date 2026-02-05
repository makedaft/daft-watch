#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>

#include "display.h"

// Config for attiny84a + external 32KHz crystal osc taken from
// https://github.com/ingepnet/breadboardwatch/blob/master/Makefile#L26
#include <avr/fuse.h>
FUSES = {
    .low = 0xe6,
    .high = 0xd7,
    .extended = 0xff,
};

void set_all_pins_output(void) {
  DDRA |= 0b11111111;
  DDRB |= 0b11111111;
}

void setup_rtc() {
  // Set prescaler (Timer clock = F_CPU / 1024)
  // For 32768Hz -> 32768/1024 = 32Hz
  TCCR0B = (1 << CS02) | (1 << CS00) | (1 << WGM02);
  TCCR0A = (1 << WGM01) | (1 << WGM00);

  TCNT0 = 0;  // Start counting from 0
  OCR0A = 31; // 0-31 = 32 counts before timer interrupt (1Hz)
  OCR0B = 0;

  // TIFR0 = (1 << TOV0);  // Clear interrupt flags
  TIFR0 = (1 << OCF0A) | (1 << OCF0B) | (1 << TOV0); // Clear interrupt flags
  // TIMSK0 = (1 << TOIE0); // Enable timer overflow interrupt
  TIMSK0 = (1 << OCIE0A); // Enable timer overflow interrupt
}

volatile uint8_t seconds = 10;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;
// Clock
ISR(TIM0_COMPA_vect, ISR_FLATTEN) {
  // if (true) {
  //   OCR0A = 32;
  // } else if (true) {
  //   OCR0A = 31;
  // } else {
  //   OCR0A = 30;
  // }

  uint8_t s = seconds + 1;
  seconds = s % 60;
  if (s >= 60) {
    uint8_t m = minutes + 1;
    minutes = m % 60;
    if (m >= 60) {
      hours = (hours + 1) % 24;
    }
  }
}

volatile static uint8_t digit_mask = 0b0001;
// Display
ISR(TIM1_COMPA_vect, ISR_FLATTEN) {
  set_all_pins_output();
  write_time(minutes, seconds, digit_mask);
  if (seconds % 2) {
    PORTA &= ~(1 << PA5);
    PORTB &= ~(1 << PB3);
  } else {
    PORTA |= (1 << PA5);
    PORTB |= (1 << PB3);
  }

  digit_mask = digit_mask << 1;
  if (digit_mask > 0b1000)
    digit_mask = 0b0001;
}

#define IS_BUTTON_ON() ((PINA & (1 << PA0)) == 0)
#define ENABLE_BUTTON() (DDRA &= ~(1 << PA0))

int main(void) {
  PRR = 0b0011; // Power reduction register. Shut down USI and ADC

  setup_rtc();
  setup_display();

  sei();

  set_all_pins_output();

  while (1) {
    // cli();
    // set_all_pins_output();
    // write_time(minutes, seconds, digit_mask);
    // if (seconds % 2) {
    //   PORTA &= ~(1 << PA5);
    //   PORTB &= ~(1 << PB3);
    // } else {
    //   PORTA |= (1 << PA5);
    //   PORTB |= (1 << PB3);
    // }
    // digit_mask = digit_mask << 1;
    // if (digit_mask > 0b1000)
    //   digit_mask = 0b0001;

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
