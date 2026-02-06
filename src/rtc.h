#ifndef _LOADED_RTC
#define _LOADED_RTC
#include <stdint.h>

volatile extern uint8_t seconds;
volatile extern uint8_t minutes;
volatile extern uint8_t hours;

void rtc_setup(void);

void rtc_increment(void);

#endif
