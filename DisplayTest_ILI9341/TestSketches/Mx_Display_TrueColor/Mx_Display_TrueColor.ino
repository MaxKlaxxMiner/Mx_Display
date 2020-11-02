#include <Mx_Display_ILI9341.h>

Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial tft(false);

void setup()
{
}

void loop()
{
  uint8_t divX = (uint8_t)(tft.width() / 16);
  uint8_t divY = (uint8_t)(tft.height() / 16);
  uint8_t redCache[tft.width()];
  for (uint16_t x = 0; x < tft.width(); x++)
  {
    redCache[x] = x * 16 / divX;
  }

  // --- default colors 16-Bit (565) ---
  for (uint16_t y = 0; y < tft.height(); y++)
  {
    uint8_t blue = y * 16 / divY / 4 + 192;
    for (uint16_t x = 0; x < tft.width(); x++)
    {
      tft.drawPixel(x, y, color565(redCache[x], 0, blue));
    }
  }
  delay(1000);

  // --- full display colors 18-Bit (666) ---
  tft.enableTrueColor();
  for (uint16_t y = 0; y < tft.height(); y++)
  {
    uint8_t blue = y * 16 / divY / 4 + 192;
    for (uint16_t x = 0; x < tft.width(); x++)
    {
      tft.setWindow(x, y, x, y); // use window-hack to disable internal dithering
      tft.fillWindowPixelTrue(redCache[x], 0, blue, 0, 0);
    }
  }
  tft.disableTrueColor();
  delay(1000);

  // --- full display color 18-Bit (666) + plus fast 2x2 dither pattern to emulate 24-Bit true colors (888) ---
  tft.enableTrueColor();
  for (uint16_t y = 0; y < tft.height(); y++)
  {
    uint8_t blue = y * 16 / divY / 4 + 192;
    for (uint16_t x = 0; x < tft.width(); x++)
    {
      tft.drawPixelTrue(x, y, redCache[x], 0, blue);
    }
  }
  tft.disableTrueColor();
  delay(1000);
}
