
#include <arduino.h>
#define __DELAY_WAIT_1MS__ F_CPU / 4000 - 3
#include "../Mx_Basics.h"

// __attribute__((always_inline))

void setup()
{
  pinModeDirect(13, OUTPUT);
  digitalWriteDirect(13, LOW);
  pinModeDirect(12, OUTPUT);
  //Serial.begin(9600);
}

void loop()
{
  digitalWriteDirect(12, HIGH);
  delayDirect(1);
  asm volatile("nop\nnop\nnop\n");
  asm volatile("nop\nnop\nnop\n");
  digitalWriteDirect(12, LOW);
  delayDirect(1);
  asm volatile("nop\nnop\n");


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
