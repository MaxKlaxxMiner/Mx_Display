
#include "../Mx_Basics.h"

// __attribute__((always_inline))

void setup()
{
  pinModeDirect(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  digitalWriteDirect(LED_BUILTIN, HIGH);
  delayDirect(50);
  digitalWriteDirect(LED_BUILTIN, LOW);
  delayDirect(950);

  // --- measure ---
  //uint32_t m1 = micros();
  //delay(10);
  //m1 = micros() - m1;
  //uint32_t m2 = micros();
  //delayDirect(10);
  //m2 = micros() - m2;
  //Serial.print(m1); Serial.print(" - "); Serial.println(m2);
  //delay(500);
}
