#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "display.h"
#include "rtc.h"

void display_setup() {
  // Set prescaler CS01 + CS00 (F_CPU / 64) + CTC
  // For 32768Hz -> 32768/64 = 512Hz
  TCCR0B = (1 << CS01) | (1 << CS00) | (1 << WGM02);
  TCCR0A = (1 << WGM01) | (1 << WGM00);

  TCNT0 = 0; // Start counting from 0
  OCR0A = 1; // 0-1 = 2 counts before timer interrupt (512/2 = 256Hz)
  OCR0B = 0;

  TIFR0 = (1 << OCF0A) | (1 << OCF0B) | (1 << TOV0); // Clear interrupt flags
  TIMSK0 = (1 << OCIE0A); // Enable timer overflow interrupt

  // Draw startup state
  DDRA |= 0b11111111;
  DDRB |= 0b11111111;
  PORTA = (1 << PA7);
  PORTB = (1 << PB2);
}

volatile uint8_t current_digit = 0;

// Render current digit
void display_render(void) {
  write_time(minutes, seconds, current_digit);

  // Cycle through the 4 digits
  current_digit = (current_digit + 1) % 4;
}

// TODO: Wtf. Fix the names

#define DSEGMENT_1 6
#define DSEGMENT_MASK_1 ((1 << 6) | (1 << 7))
#define DSEGMENT_2 5
#define DSEGMENT_MASK_2 ((1 << 5) | (1 << 10))

#define ALL_DUAL_PURPOSE_PINS (DSEGMENT_MASK_1 | DSEGMENT_MASK_2)

uint8_t dual_purpose_pins[] = {6, 7, 5, 10};
uint8_t dual_purpose_antipins[] = {7, 6, 10, 5};

void write_time(uint16_t hour, uint16_t minute, uint8_t digit_index) {
  // reset pins
  uint32_t ddr = ~ALL_DUAL_PURPOSE_PINS;
  uint32_t port = 0;

  uint8_t digit = get_digit(hour, minute, digit_index);
  uint32_t segments = get_digit_mask(digit);
  uint8_t pin = dual_purpose_pins[digit_index];
  uint8_t antipin = dual_purpose_antipins[digit_index];
  uint8_t segment_pin = digit_index / 2 == 0 ? DSEGMENT_1 : DSEGMENT_2;
  uint8_t other_segment_pin = digit_index / 2 == 0 ? DSEGMENT_2 : DSEGMENT_1;
  uint8_t other_segment_mask =
      digit_index / 2 == 0 ? DSEGMENT_MASK_2 : DSEGMENT_MASK_1;

  port = segments;

  // Activate dual purpose segment
  ddr |= (1 << pin);
  port |= (1 << pin);
  if (segments & (1 << segment_pin)) {
    // If seg inactive, enable other input and enable pull up res
    ddr &= ~(1 << antipin);
    port |= (1 << antipin);
  } else {
    // If seg active, enable other output and set to 0
    ddr |= (1 << antipin);
    port &= ~(1 << antipin);
  }

  // Activate other dual purpose segment
  if ((segments & (1 << other_segment_pin)) == 0) {
    ddr |= other_segment_mask;
    port &= ~other_segment_mask;
  }

  // cli();
  DDRA = ddr;
  DDRB = ddr >> 8;
  PORTA = port;
  PORTB = port >> 8;
  // sei();
}
