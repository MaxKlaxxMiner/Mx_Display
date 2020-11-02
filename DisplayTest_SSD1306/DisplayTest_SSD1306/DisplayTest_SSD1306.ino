#include "Max_SSD1306.h"
Max_SSD1306 display;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.begin(0x3c, 10000000UL);
  //display.dim(1);
  //display.pump(0);
  //display.invertDisplay(1);

  display.clearDisplay();
  display.drawPixel(0, 0, 1);
  display.drawPixel(10, 10, 1);
  display.drawPixel(127, 63, 1);
  display.display();
  delay(500);

  uint32_t tick = 0;
  uint32_t mess = 18800; // 18758 - Max_SSD1306

  for (; ; )
  {
    uint32_t m = micros();
    tick = m - tick;
    if (tick < mess) mess--; else mess++;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(mess);
    for (byte c = 14; c < 161; c++)
    {
      display.write(c);
    }
    display.display();
    tick = m;
  }
}

void loop()
{
}
