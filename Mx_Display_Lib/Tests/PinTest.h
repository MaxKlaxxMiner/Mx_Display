
#include <arduino.h>
#define __DELAY_WAIT_1MS__ F_CPU / 4000 - 3
#include "../Mx_Basics.h"

// __attribute__((always_inline))

void setup()
{
  pinModeDirect(13, OUTPUT);
  digitalWriteDirect(13, LOW);
}

void loop()
{
  delay(1000);
}
