// definetest.h

#ifndef _DEFINETEST_h
#define _DEFINETEST_h

#include "arduino.h"

#ifdef blink_long
#define blink                      \
  digitalWrite(LED_BUILTIN, HIGH); \
  delay(950);                      \
  digitalWrite(LED_BUILTIN, LOW);  \
  delay(50);
#else
#define blink                      \
  digitalWrite(LED_BUILTIN, HIGH); \
  delay(50);                       \
  digitalWrite(LED_BUILTIN, LOW);  \
  delay(950);
#endif



#endif

