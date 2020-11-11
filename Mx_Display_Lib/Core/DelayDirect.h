
#ifndef _MX_DELAY_DIRECT_h
#define _MX_DELAY_DIRECT_h

#if F_CPU > 20000

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega2560__)
#define __DELAY_WAIT_1MS__ (F_CPU / 4000 - 3)
#endif
#if defined(__SAM3X8E__)
#define __DELAY_WAIT_1MS__ (F_CPU / 5250 - 3)
#endif

#endif

#ifdef __DELAY_WAIT_1MS__
void delayDirect(uint16_t ms)
{
  for (; ms > 0; ms--)
  {
    for (uint16_t i = 0; i < __DELAY_WAIT_1MS__; i++) asm volatile("\n");
  }
}
#else
// --- vanilla ---
#define delayDirect(ms) delay((ms))
#endif

#endif _MX_DELAY_DIRECT_h
