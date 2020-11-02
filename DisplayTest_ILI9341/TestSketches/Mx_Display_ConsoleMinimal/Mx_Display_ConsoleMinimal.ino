#include <Mx_Display_ILI9341.h>
#include <Mx_Display_Scrollable.h>
#include <Mx_Display_Console.h>

Mx_DisplayV_ILI9341_CompactMinimal_NoSd_NoSerial tftBase(false);
Mx_Display_Scrollable<Mx_DisplayV_ILI9341_CompactMinimal_NoSd_NoSerial> tft(&tftBase);
Mx_Display_Console_CompactMinimal<Mx_DisplayV_ILI9341_CompactMinimal_NoSd_NoSerial> txt(&tft);

int main()
{
  init(); // need for millis()

  // --- these loops are smaller than tft.clear(0x0000); ---
  for (int y = 0; y < tft.height(); y++)
  {
    for (int x = 0; x < tft.width(); x++)
    {
      tft.drawPixel(x, y, 0x0000);
    }
  }

  uint32_t count = 0;
  while(true)
  {
    txt.print(count++); txt.print(" - Hello World! - "); txt.println(millis());
  }
  
  return 0;
}
