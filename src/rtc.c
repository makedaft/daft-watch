#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "rtc.h"

void rtc_setup(void) {
  // Set prescaler CS12 + CS10 (F_CPU / 1024) + CTC
  // For 32768Hz -> 32768/1024 = 32Hz
  TCCR1B = (1 << CS12) | (1 << CS10) | (1 << WGM12) | (1 << WGM13);
  TCCR1A = (1 << WGM11) | (1 << WGM10);

  TCNT1 = 0;  // Start counting from 0
  OCR1A = 31; // 0-31 = 32 counts before timer interrupt (1Hz)
  OCR1B = 0;

  TIFR1 = (1 << OCF1A) | (1 << OCF1B) | (1 << TOV1); // Clear interrupt flags
  TIMSK1 = (1 << OCIE1A); // Enable timer overflow interrupt
}

volatile uint8_t seconds = 10;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;

void rtc_increment(void) {
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
