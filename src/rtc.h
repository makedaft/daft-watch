#ifndef _LOADED_RTC
#define _LOADED_RTC
#include <stdint.h>

volatile extern uint8_t seconds;
volatile extern uint8_t minutes;
volatile extern uint8_t hours;

volatile extern uint8_t seconds_digit1;
volatile extern uint8_t seconds_digit2;
volatile extern uint8_t minutes_digit1;
volatile extern uint8_t minutes_digit2;
volatile extern uint8_t hours_digit1;
volatile extern uint8_t hours_digit2;

void rtc_setup(void);

void rtc_increment(void);

void rtc_set_minutes(uint8_t m);

void rtc_set_hours(uint8_t h);

#endif
