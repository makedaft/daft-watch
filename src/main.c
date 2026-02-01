#include <avr/io.h>
#include <util/delay.h>

// gfedcba
const long digits[] = {
    0b0111111, 0b0000110, 0b1011011, 0b1001111, 0b1100110,
    0b1101101, 0b1111101, 0b0000111, 0b1111111, 0b1101111,
};

//                          a, b, c, d, e, f, g
const int segment_pins[] = {4, 5, 6, 7, 8, 9, 10};

void write_digit(int digit) {
  if (digit > 9 || digit < 0)
    return;

  long mask = digits[digit];
  for (int i = 0; i < 7; i++) {
    short on = 0 != (mask & (i == 0 ? 1 : 2 << (i - 1)));
    // digitalWrite(segment_pins[i], on);
  }
}

void setup(void) {
  for (int i = 0; i < 7; i++) {
    int pin = segment_pins[i];
    DDRA |= (1 << PA1);
  }
}

int i = 0;
void loop(void) {
  write_digit(i);
  _delay_ms(500);
  i = (i + 1) % 10;
}

int main(void) {
  setup();

  while (1) {
    loop();
    // PORTA |= (1 << PA1);
    // _delay_ms(500);
    // PORTA &= ~(1 << PA1);
    // _delay_ms(500);
  }

  return 0;
}
