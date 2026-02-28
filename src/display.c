#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "display.h"
#include "mode.h"
#include "rtc.h"

// "_gfedcba"
// Reversed because a-g is mapped to pa0-6. (pa7 is decimal)
// Complemented because display is common anode
static const uint32_t digit_masks[] = {
    0b1000000, 0b1111001, 0b0100100, 0b0110000, 0b0011001,
    0b0010010, 0b0000010, 0b1111000, 0b0000000, 0b0010000,
};

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

void write_time(uint8_t digit_index, const uint8_t digits[4]);

// Current digit position
volatile uint8_t current_digit = 0;

// Render current digit
void display_render(void) {
  switch (current_mode) {
  case ModeDisplayTime: {
    uint8_t digits[] = {minutes_digit1, minutes_digit2, seconds_digit1,
                        seconds_digit2};
    write_time(current_digit, digits);
    break;
  }
  case ModeSetTime: {
    uint8_t digits[] = {new_hours / 10, new_hours % 10, new_minutes / 10,
                        new_minutes % 10};
    write_time(current_digit, digits);
    break;
  }
  }

  // Cycle through the 4 digits
  current_digit = (current_digit + 1) % 4;
}

// Special segments index position
#define SPECIAL_SEGMENT_1 5
#define SPECIAL_SEGMENT_2 6

// Special segment masked pins
#define SPECIAL_SEGMENT_MASK_1 ((1 << 5) | (1 << 10))
#define SPECIAL_SEGMENT_MASK_2 ((1 << 6) | (1 << 7))

// All special segment pings
#define SPECIAL_SEGMENT_PINS (SPECIAL_SEGMENT_MASK_1 | SPECIAL_SEGMENT_MASK_2)

// Digit position -> special segment pin
static uint8_t special_segment_pins[] = {5, 10, 6, 7};

// Digit position -> special segment anti-pin (pair of pin)
static uint8_t special_segment_antipins[] = {10, 5, 7, 6};

void write_time(uint8_t digit_index, const uint8_t digits[4]) {
  // reset pins
  uint16_t ddr = ~SPECIAL_SEGMENT_PINS;
  uint16_t port = 0;

  uint8_t digit = digits[digit_index];
  uint16_t segments = digit_masks[digit];
  uint8_t pin = special_segment_pins[digit_index];
  uint8_t antipin = special_segment_antipins[digit_index];
  uint8_t segment_pin =
      digit_index / 2 == 0 ? SPECIAL_SEGMENT_1 : SPECIAL_SEGMENT_2;
  uint8_t other_segment_pin =
      digit_index / 2 == 0 ? SPECIAL_SEGMENT_2 : SPECIAL_SEGMENT_1;
  uint16_t other_segment_mask =
      digit_index / 2 == 0 ? SPECIAL_SEGMENT_MASK_2 : SPECIAL_SEGMENT_MASK_1;

  // Activate special segment
  ddr |= (1 << pin);
  port = segments | (1 << pin);
  if (segments & (1 << segment_pin)) {
    // If seg inactive, enable other input and enable pull up res
    ddr &= ~(1 << antipin);
    port |= (1 << antipin);
  } else {
    // If seg active, enable other output and set to 0
    ddr |= (1 << antipin);
    port &= ~(1 << antipin);
  }

  // Activate other special segment
  if ((segments & (1 << other_segment_pin)) == 0) {
    ddr |= other_segment_mask;
    port &= ~other_segment_mask;
  }

  DDRA = ddr;
  PORTA = port;
  DDRB = ddr >> 8;
  PORTB = port >> 8;
}
