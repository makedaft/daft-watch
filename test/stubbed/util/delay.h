#ifndef STUB_UTIL_DELAY
#define STUB_UTIL_DELAY

#include <unistd.h>

static void _delay_ms(int ms) { usleep(ms * 1000); }
#endif
