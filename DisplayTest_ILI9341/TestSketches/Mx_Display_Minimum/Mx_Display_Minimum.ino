#include <Mx_Display_ILI9341.h>

// --- fastest: with maximum speed (recommended) ---
//     info: remove the "_NoSd" and/or "_NoSerial" if you want to use an SD card or the hardware Serial port (reduces the display performance)
Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial tft(false);

// --- fast: good speed + less flash memory usage ---
// Mx_DisplayV_ILI9341_Fast_NoSd_NoSerial tft(false);

// --- compact: acceptable speed, useful if you need to save more flash memory ---
// Mx_DisplayV_ILI9341_Compact_NoSd_NoSerial tft(false);

// --- compact-minimal: requires the least amount of flash memory, but is also much slower ---
// Mx_DisplayV_ILI9341_CompactMinimal_NoSd_NoSerial tft(false);

// --- reference design for highest compatibility: but it's very slow (use digitalWrite() etc.), only suitable for test and debug purposes ---
// Mx_DisplayV_ILI9341_Reference tft(false);

void setup()
{
  // no further settings required
}

void loop()
{
  tft.clear(color565(0, 0, 0));     // fill screen: black
  delay(1000);                      // wait 1 second
    
  tft.clear(color565(0, 128, 255)); // fill screen: light blue
  delay(1000);                      // wait 1 second
}
