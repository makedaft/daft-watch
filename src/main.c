#include <avr/io.h>
#include <util/delay.h>

// a b c d e f g decimal
// Reversed because a-g is mapped to pa0-6. (pa7 is decimal)
// Complemented because display is common anode
const long digits[] = {
    0b01000000, 0b01111001, 0b00100100, 0b00110000, 0b00011001,
    0b00010010, 0b00000010, 0b01111000, 0b00000000, 0b00010000,
};

void write_digit(int digit) {
  if (digit > 9 || digit < 0)
    return;

  PORTA = digits[digit];
}

int main(void) {
  DDRA |= 0b11111111;

  int i = 0;
  while (1) {
    write_digit(i);
    _delay_ms(500);
    i = (i + 1) % 10;
  }
  return 0;
}
