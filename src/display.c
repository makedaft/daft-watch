#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "display.h"

// "_gfedcba"
// Reversed because a-g is mapped to pa0-6. (pa7 is decimal)
// Complemented because display is common anode
const uint32_t digit_masks[] = {
    0b01000000, 0b01111001, 0b00100100, 0b00110000, 0b00011001,
    0b00010010, 0b00000010, 0b01111000, 0b00000000, 0b00010000,
};

void setup_display() {
  TIMSK1 = 0;

  // Set prescaler (Timer clock = F_CPU / 64)
  // For 32768Hz -> 32768/64 = 512Hz
  TCCR1B = (1 << CS11) | (1 << CS10) | (1 << WGM12) | (1 << WGM13);
  TCCR1A = (1 << WGM11) | (1 << WGM10);

  TCNT1 = 0; // Start counting from 0
  OCR1A = 1; // 0-1 = 2 counts before timer interrupt (512/2 = 256Hz)
  OCR1B = 0;

  TIFR1 = (1 << OCF1A) | (1 << OCF1B) | (1 << TOV1); // Clear interrupt flags
  TIMSK1 = (1 << OCIE1A); // Enable timer overflow interrupt

  // Draw startup state
  DDRA |= 0b11111111;
  DDRB |= 0b11111111;
  PORTA = (1 << PA7);
  PORTB = (1 << PB2);
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

  // _delay_ms(4);
  // Reset digit control pins
  PORTA &= ~(1 << PA7); // minute digit 1
  PORTB &= ~(1 << PB2); // minute digit 2
  // _delay_ms(4);

  // cli();
  if (mask & 0b0001 || mask & 0b0100) {
#ifdef STUBBED
    printf(":: %d\n", minute_digit1);
    fflush(stdout);
#endif
    write_digit(minute_digit1);
    PORTA |= (1 << PA7);
  }

  if (mask & 0b0010 || mask & 0b1000) {
#ifdef STUBBED
    printf(":: %d\n", minute_digit2);
    fflush(stdout);
#endif
    write_digit(minute_digit2);
    PORTB |= (1 << PB2);
  }
  // sei();
}
