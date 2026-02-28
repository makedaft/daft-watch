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

volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;

volatile uint8_t seconds_digit1 = 0;
volatile uint8_t seconds_digit2 = 0;
volatile uint8_t minutes_digit1 = 0;
volatile uint8_t minutes_digit2 = 0;
volatile uint8_t hours_digit1 = 0;
volatile uint8_t hours_digit2 = 0;

void rtc_increment(void) {
  uint8_t s = seconds + 1;
  seconds = s % 60;
  // TODO: Remove seconds digits calculation
  seconds_digit1 = seconds % 10;
  seconds_digit2 = (seconds / 10) % 10;
  if (s >= 60) {
    uint8_t m = minutes + 1;
    rtc_set_minutes(m);
    if (m >= 60)
      rtc_set_hours(hours + 1);
  }
}

void rtc_set_minutes(uint8_t m) {
  minutes = m % 60;
  minutes_digit1 = minutes % 10;
  minutes_digit2 = (minutes / 10) % 10;
}

void rtc_set_hours(uint8_t h) {
  hours = h % 24;
  hours_digit1 = hours % 10;
  hours_digit2 = (hours / 10) % 10;
}
