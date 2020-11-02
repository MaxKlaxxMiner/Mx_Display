#include <Mx_Display_ILI9341.h>
#include <Mx_Display_Scrollable.h>
#include <Mx_Display_Console.h>

Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial tftBase(false);
Mx_Display_Scrollable<Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial> tft(&tftBase);
Mx_Display_Console_Fastest<Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial> txt(&tft);

void setup()
{
  tft.clear(0x0000);
}

uint32_t count = 0;

void loop()
{
  txt.print(count++); txt.print(" - Hello World! - "); txt.println(millis());
}
