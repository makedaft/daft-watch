#include <avr/io.h>
#include <avr/fuse.h>
#include <stdint.h>
#include <util/delay.h>

#include "display.h"

// Config for attiny84a + 32KHz crystal osc taken from https://github.com/ingepnet/breadboardwatch/blob/master/Makefile#L26
FUSES = {
  .low = 0xe6,
  .high = 0xd7,
  .extended = 0xff,
};

void setup_output(void) {
  DDRA |= 0b11111111;
  DDRB |= 0b11111111;
}

int main(void) {
  uint32_t count = 0;
  uint8_t is_button_on = 0;

  while (1) {
    DDRB &= ~(1 << PB2);
    is_button_on = (PINB & (1 << PB2)) > 0;

    if (is_button_on) {
      _delay_ms(30);
      is_button_on = (PINB & (1 << PB2)) > 0;
      if (is_button_on) {
        setup_output();
        PORTA |= 0b10000000;
        _delay_ms(500);
        continue;
      }
    }

    setup_output();
    write_time(0, (count / 20) % 60);

    count = (count + 1) % 100000;
  }
  return 0;
}
