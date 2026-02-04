#ifndef STUB_AVR_FUSE
#define STUB_AVR_FUSE

struct __fuses_t {
  int high;
  int low;
  int extended;
};

#define FUSES const struct __fuses_t _watch_mcu_fuses_configuration

#endif
