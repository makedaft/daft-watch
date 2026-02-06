#define ISR(INTER, ...) void generated_interrupt_##INTER()

static void cli() {}
static void sei() {}

#define stub_run_timers()                                                      \
  {                                                                            \
    if (TCNT0 == OCR0A) {                                                      \
      generated_interrupt_TIM0_COMPA_vect();                                   \
      TCNT0 = 0;                                                               \
    } else {                                                                   \
      TCNT0++;                                                                 \
    }                                                                          \
    if (TCNT1 == OCR1A) {                                                      \
      generated_interrupt_TIM1_COMPA_vect();                                   \
      TCNT1 = 0;                                                               \
    } else {                                                                   \
      TCNT1++;                                                                 \
    }                                                                          \
  }
