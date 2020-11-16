
#include <arduino.h>
#define __DELAY_WAIT_1MS__ F_CPU / 4000 - 3
#include "../Mx_Basics.h"

// __attribute__((always_inline))

void setup()
{
  pinModeDirect(13, OUTPUT);
  digitalWriteDirect(13, LOW);
  //Serial.begin(9600);
  //Serial.begin(115200);
  Serial.begin(2000000);
}

void loop()
{
  //Serial.print("lol - ");
  //Serial.println(millis());
  //for (int i = 0; i < 256; i++)
  //{
  //  Serial.write((uint8_t)i);
  //  Serial.write((uint8_t)(i >> 8));
  //}
  //Serial.println();
  //delay(1000);
}
