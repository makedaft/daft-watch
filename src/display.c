#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "display.h"
#include "rtc.h"

// "_gfedcba"
// Reversed because a-g is mapped to pa0-6. (pa7 is decimal)
// Complemented because display is common anode
const uint32_t digit_masks[] = {
    0b01000000, 0b01111001, 0b00100100, 0b00110000, 0b00011001,
    0b00010010, 0b00000010, 0b01111000, 0b00000000, 0b00010000,
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

// Render current digit
void display_render(void) {
  DDRA |= 0b11111111;
  DDRB |= 0b11111111;

  write_time(minutes, seconds, current_digit);

  // Next digit
  current_digit = (current_digit + 1) % 4;
}

void write_digit(uint8_t digit) {
  if (digit > 9 || digit < 0)
    return;
  PORTA = digit_masks[digit];
}

void write_time(uint16_t hour, uint16_t minute, uint8_t mask) {
  uint8_t minute_digit1 = minute % 10;
  uint8_t minute_digit2 = (minute / 10) % 10;
  // unsigned short hour_digit1 = hour % 10;
  // unsigned short hour_digit2 = (hour / 10) % 10;

  // Reset digit control pins
  PORTA &= ~(1 << PA7); // minute digit 1
  PORTB &= ~(1 << PB2); // minute digit 2

  // cli();
  if (mask == 0 || mask == 2) {
#ifdef STUBBED
    printf(" %d\n", minute_digit1);
    fflush(stdout);
#endif
    write_digit(minute_digit1);
    PORTA |= (1 << PA7);
  }

  if (mask == 1 || mask == 3) {
#ifdef STUBBED
    printf(":: %d", minute_digit2);
    fflush(stdout);
#endif
    write_digit(minute_digit2);
    PORTB |= (1 << PB2);
  }
  // sei();
}
