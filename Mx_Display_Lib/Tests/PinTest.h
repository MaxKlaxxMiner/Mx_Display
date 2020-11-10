
#include "../Mx_Basics.h"

// __attribute__((always_inline))

void setup()
{
  pinModeDirect(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWriteDirect(LED_BUILTIN, HIGH);
  delayDirect(50);
  digitalWriteDirect(LED_BUILTIN, LOW);
  delayDirect(950);
}
