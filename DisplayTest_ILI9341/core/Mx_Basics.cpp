#include "Mx_Basics.h"

#ifdef __DELAY_WAIT_1MS__
void delayDirect(uint16_t ms)
{
  for (; ms > 0; ms--)
  {
    for (uint16_t i = 0; i < (uint16_t)(__DELAY_WAIT_1MS__); i++) asm volatile("nop\n" ::);
  }
}
#endif
