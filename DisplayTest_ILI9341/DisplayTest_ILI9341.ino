#include "Mx_Display_ILI9341.h"

Mx_DisplayV_ILI9341_MOD_Fastest_NoSerial tft(false);
//Mx_DisplayV_ILI9341_Reference tft(false);

void trueTest()
{
  tft.enableTrueColor();
  int r1 = 255;
  int g1 = 0;
  int b1 = 0;

  int r2 = 0;
  int g2 = 255;
  int b2 = 0;

  int r3 = 0;
  int g3 = 0;
  int b3 = 255;

  int r4 = 255;
  int g4 = 255;
  int b4 = 255;

  for (int y = 0; y < tft.height(); y++)
  {
    for (int x = 0; x < tft.width(); x++)
    {
      uint32_t ra = (x * (uint32_t)r2 + (tft.width() - x - 1) * (uint32_t)r1);
      uint32_t ga = (x * (uint32_t)g2 + (tft.width() - x - 1) * (uint32_t)g1);
      uint32_t ba = (x * (uint32_t)b2 + (tft.width() - x - 1) * (uint32_t)b1);
      uint32_t rb = (x * (uint32_t)r4 + (tft.width() - x - 1) * (uint32_t)r3);
      uint32_t gb = (x * (uint32_t)g4 + (tft.width() - x - 1) * (uint32_t)g3);
      uint32_t bb = (x * (uint32_t)b4 + (tft.width() - x - 1) * (uint32_t)b3);
      uint32_t r = (y * (uint32_t)rb + (tft.height() - y - 1) * (uint32_t)ra) / (tft.width() * (uint32_t)tft.height());
      uint32_t g = (y * (uint32_t)gb + (tft.height() - y - 1) * (uint32_t)ga) / (tft.width() * (uint32_t)tft.height());
      uint32_t b = (y * (uint32_t)bb + (tft.height() - y - 1) * (uint32_t)ba) / (tft.width() * (uint32_t)tft.height());
      tft.drawPixelTrue(x, y, r, g, b);
    }
  }
}

void setup()
{
  pinModeDirect(LED_BUILTIN, OUTPUT);
  digitalWriteDirect(LED_BUILTIN, LOW);

  tft.clear(0x0000);

  tft.drawCircle(120, 160, 50, 0xffff);

  tft.command16(ILI9341_COLUMN_ADDRESS_SET, 120, 122);
  tft.command16(ILI9341_PAGE_ADDRESS_SET, 160, 160);

  tft.command(ILI9341_MEMORY_WRITE);
  tft.write8(0xff); tft.writeStrobe();
  tft.write8(0xff); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0xff); tft.writeStrobe();
  tft.write8(0xff); tft.writeStrobe();

  tft.command16(ILI9341_PAGE_ADDRESS_SET, 162, 164);
  tft.command(ILI9341_MEMORY_WRITE);
  tft.write8(0xff); tft.writeStrobe();
  tft.write8(0xff); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0xff); tft.writeStrobe();
  tft.write8(0xff); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0xff); tft.writeStrobe();
  tft.write8(0xff); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0x00); tft.writeStrobe();
  tft.write8(0xff); tft.writeStrobe();
  tft.write8(0xff); tft.writeStrobe();
}

void loop()
{
}
