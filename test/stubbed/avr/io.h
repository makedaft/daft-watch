#ifndef STUB_AVR_IO
#define STUB_AVR_IO
#include <stdio.h>

static int DDRA = 0;
static int DDRB = 0;

static int PORTA = 0;
static int PORTB = 0;
static int PINA = 0;
static int PINB = 0;

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

static int PRR = 0;
static int TCCR0B = 0;
static int TCCR0A = 0;
static int TIMSK0 = 0;
static int TIFR0 = 0;
static int OCR0A = 0;
static int OCR0B = 0;
static int TCNT0 = 0;
static int TCCR1B = 0;
static int TCCR1A = 0;
static int TIMSK1 = 0;
static int TIFR1 = 0;
static int OCR1A = 0;
static int OCR1B = 0;
static int TCNT1 = 0;
static int TCNT1H = 0;
static int TCNT1L = 0;

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
