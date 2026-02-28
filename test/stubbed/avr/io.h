#ifndef STUB_AVR_IO
#define STUB_AVR_IO
#include <stdio.h>

extern int DDRA;
extern int DDRB;

extern int PORTA;
extern int PORTB;
extern int PINA;
extern int PINB;

#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7

#define PB1 1
#define PB2 2
#define PB3 3
#define PB4 4

extern int PRR;
extern int TCCR0B;
extern int TCCR0A;
extern int TIMSK0;
extern int TIFR0;
extern int OCR0A;
extern int OCR0B;
extern int TCNT0;
extern int TCCR1B;
extern int TCCR1A;
extern int TIMSK1;
extern int TIFR1;
extern int OCR1A;
extern int OCR1B;
extern int TCNT1;
extern int TCNT1H;
extern int TCNT1L;

#define CS00 1
#define CS01 1
#define CS02 2
#define CS10 1
#define CS11 1
#define CS12 2
#define WGM00 0
#define WGM01 1
#define WGM02 2
#define WGM10 0
#define WGM11 1
#define WGM12 2
#define WGM13 3
#define TOIE0 0
#define TOIE1 0
#define TOV0 0
#define TOV1 0
#define OCF1A 0
#define OCF1B 0
#define OCF0A 0
#define OCF0B 0
#define OCIE0A 0
#define OCIE0B 0
#define OCIE1A 0
#define OCIE1B 0
#endif
