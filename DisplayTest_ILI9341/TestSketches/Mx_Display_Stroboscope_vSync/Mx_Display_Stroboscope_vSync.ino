#include <Mx_Display_ILI9341.h>

Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial tft(false);

void setup()
{
}

void loop()
{
  tft.clear(color565(0, 0, 0)); // fill screen: black
  tft.vSync(2);                 // vertical sync, parameter for scan-line fine tuning (0-323)
    
  tft.clear(color565(255, 255, 255)); // fill screen: white
  tft.vSync(2);
}
