#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "button.h"
#include "display.h"
#include "rtc.h"

// "_gfedcba"
// Reversed because a-g is mapped to pa0-6. (pa7 is decimal)
// Complemented because display is common anode
const uint32_t digit_masks[] = {
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

volatile uint8_t current_digit = 0;
#define TOTAL_DIGITS 4

// Render current digit
void display_render(void) {
  // button_tick(); // Check button click

  write_time(minutes, seconds, current_digit);

  // Cycle through the 4 digits
  current_digit = (current_digit + 1) % TOTAL_DIGITS;
}

inline uint32_t get_digit(uint8_t digit) {
  return digit_masks[digit]; // digit % 10
}

void write_time(uint16_t hour, uint16_t minute, uint8_t digit_index) {
  uint8_t minute_digit1 = minute % 10;
  uint8_t minute_digit2 = (minute / 10) % 10;
  // unsigned short hour_digit1 = hour % 10;
  // unsigned short hour_digit2 = (hour / 10) % 10;

  // reset pins (all output except PA7,PB2)
  uint16_t ddra = ~((1 << PA7));
  uint16_t ddrb = ~((1 << PB2));
  uint16_t porta = 0;
  uint16_t portb = 0;

  if (digit_index == 0 || digit_index == 2) {
    uint32_t segments = get_digit(minute_digit1);
    porta = segments;
    ddra |= (1 << PA7);
    porta |= (1 << PA7);
    if (segments & (1 << 6)) {
      // If seg inactive, enable other input and enable pull up res
      ddrb &= ~(1 << PB2);
      portb |= (1 << PB2);
    } else {
      // If seg active, enable other output and set to 0
      ddrb |= (1 << PB2);
      portb &= ~(1 << PB2);
    }
  }

  if (digit_index == 1 || digit_index == 3) {
    uint32_t segments = get_digit(minute_digit2);
    porta = segments;
    ddrb |= (1 << PB2);
    portb |= (1 << PB2);
    if (segments & (1 << 6)) {
      // If seg inactive, enable other input and enable pull up res
      ddra &= ~(1 << PA7);
      porta |= (1 << PA7);
    } else {
      // If seg active, enable other output and set to 0
      ddra |= (1 << PA7);
      porta &= ~(1 << PA7);
    }
  }

  // cli();
  DDRA = ddra;
  DDRB = ddrb;
  PORTA = porta;
  PORTB = portb;
  // sei();
}
