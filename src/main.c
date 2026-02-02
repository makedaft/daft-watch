#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

// a b c d e f g decimal
// Reversed because a-g is mapped to pa0-6. (pa7 is decimal)
// Complemented because display is common anode
const uint32_t digits[] = {
    0b01000000, 0b01111001, 0b00100100, 0b00110000, 0b00011001,
    0b00010010, 0b00000010, 0b01111000, 0b00000000, 0b00010000,
};

void write_digit(uint8_t digit) {
  if (digit > 9 || digit < 0)
    return;
  PORTA = digits[digit];
}

void write_time(uint16_t hour, uint16_t minute) {
  uint8_t minute_digit1 = minute % 10;
  uint8_t minute_digit2 = minute / 10;
  // unsigned short hour_digit1 = hour % 10;
  // unsigned short hour_digit2 = hour / 10;

#ifdef STUBBED
  printf("\r:: %d %d", minute_digit2, minute_digit1);
  fflush(stdout);
#endif

  write_digit(minute_digit1);
  PORTA |= (1 << PA7);
  _delay_ms(10);
  PORTA &= ~(1 << PA7);

  write_digit(minute_digit2);
  PORTB |= (1 << PB2);
  _delay_ms(10);
  PORTB &= ~(1 << PB2);
}

int main(void) {
  DDRA |= 0b11111111;
  DDRB |= (1 << PB2);

  uint32_t i = 0;
  while (1) {
    write_time(0, (i / 20) % 60);
    i = (i + 1) % 100000;
  }
  return 0;
}
