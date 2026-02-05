#define ISR(INTER) void generated_interrupt_##INTER()

static void cli() {}
static void sei() {}

#define stub_run_timers()                                                      \
  {                                                                            \
    if (TCNT0 == OCR0A) {                                                      \
      generated_interrupt_TIM0_OVF_vect();                                     \
      TCNT0 = 0;                                                               \
    } else {                                                                   \
      TCNT0++;                                                                 \
    }                                                                          \
    if (TCNT1L == OCR1AH) {                                                    \
      generated_interrupt_TIM1_OVF_vect();                                     \
      TCNT1L = 0;                                                              \
    } else {                                                                   \
      TCNT1L++;                                                                \
    }                                                                          \
  }
