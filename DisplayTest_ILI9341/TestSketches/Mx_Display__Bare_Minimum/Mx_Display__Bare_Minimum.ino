#include <Mx_Display_ILI9341.h>

Mx_DisplayV_ILI9341_CompactMinimal_NoSd_NoSerial tft(false);

int main() // replace setup() and loop()
{
  // init(); // initialized arduino library (if you need e.g. millis(), analogRead() etc.)
  
  while (true)
  {
    tft.clear(color565(0, 0, 0));     // fill screen: black
    delayDirect(1000);                // wait 1 second
    
    tft.clear(color565(0, 128, 255)); // fill screen: light blue
    delayDirect(1000);                // wait 1 second
  }
  return 0;
}
