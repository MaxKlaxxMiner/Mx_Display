#include <Mx_Display_ILI9341.h>
#include <Mx_Display_Scrollable.h>

Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial tftBase(false);
Mx_Display_Scrollable<Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial> tft(&tftBase);

const int boxWidth = 77;
const int boxHeight = 77;
int posX = -20;
int posY = -20;
int moveX = 3;
int moveY = 3;
uint16_t timer = 540;

byte* xCache;
byte* yCache;

void setup()
{
  for (int x = 0; x < tft.width(); x++)
  {
    tft.drawLine(x, 0, 0, tft.height() - 1, color565(0, 0, x / 4));
  }
  for (int y = 0; y < tft.height(); y++)
  {
    tft.drawLine(tft.width() - 1, y, 0, tft.height() - 1, color565(0, 0, 60 + y / 5));
  }
  delay(500);

  tft.enableTrueColor();

  // --- draw "true" blue lines ---
  for (int x = 0; x < tft.width(); x++)
  {
    tft.drawLineTrue(x, 0, 0, tft.height() - 1, 0, 0, x / 4);
  }
  for (int y = 0; y < tft.height(); y++)
  {
    tft.drawLineTrue(tft.width() - 1, y, 0, tft.height() - 1, 0, 0, 60 + y / 5);
  }
  delay(1000);

  // --- fill color-cache ---
  xCache = new byte[tft.width()];
  for (int x = 0; x < tft.width(); x++)
  {
    xCache[x] = x * 16 / (byte)(tft.width() / 16);
  }
  yCache = new byte[tft.height()];
  for (int y = 0; y < tft.height(); y++)
  {
    yCache[y] = (y * 16 / (byte)(tft.height() / 4)) + 192;
  }
}

void loop()
{
  if (posX + moveX < -20 || (posX + boxWidth) + moveX > tft.width() + 20) moveX = -moveX;
  if (posY + moveY < -20 || (posY + boxHeight) + moveY > tft.height() + 20) moveY = -moveY;
  
  if (moveX >= 0)
  {
    for (int y = -1; y < boxHeight; y++)
    {
      byte yy = yCache[posY + y];
      for (byte x = 0; x < moveX; x++)
      {
        tft.drawPixelTrue(posX + x, posY + y, xCache[posX + x], 0, yy);
      }
    }
    if (moveY >= 0)
    {
      for (int y = 0; y < moveY; y++)
      {
        byte yy = yCache[posY + y];
        uint16_t ys = tft.setWindowLineSafe(posX + moveX, posY + y, posX + boxWidth - 1);
        for (byte x = moveX; x < boxWidth; x++)
        {
          tft.fillWindowPixelTrueSafe(xCache[posX + x], 0, yy, posX + x, ys);
        }
      }
    }
    else
    {
      for (int y = boxHeight + moveY; y < boxHeight; y++)
      {
        byte yy = yCache[posY + y];
        uint16_t ys = tft.setWindowLineSafe(posX + moveX, posY + y, posX + boxWidth - 1);
        for (byte x = moveX; x < boxWidth; x++)
        {
          tft.fillWindowPixelTrueSafe(xCache[posX + x], 0, yy, posX + x, ys);
        }
      }
    }
  }
  else
  {
    for (int y = -1; y < boxHeight; y++)
    {
      byte yy = yCache[posY + y];
      for (byte x = boxWidth + moveX; x < boxWidth; x++)
      {
        tft.drawPixelTrue(posX + x, posY + y, xCache[posX + x], 0, yy);
      }
    }
    if (moveY >= 0)
    {
      for (int y = 0; y < moveY; y++)
      {
        byte yy = yCache[posY + y];
        uint16_t ys = tft.setWindowLineSafe(posX, posY + y, posX + boxWidth - 1);
        for (byte x = 0; x < boxWidth; x++)
        {
          tft.fillWindowPixelTrueSafe(xCache[posX + x], 0, yy, posX + x, ys);
        }
      }
    }
    else
    {
      for (int y = boxHeight + moveY; y < boxHeight; y++)
      {
        byte yy = yCache[posY + y];
        uint16_t ys = tft.setWindowLineSafe(posX, posY + y, posX + boxWidth - 1);
        for (byte x = 0; x < boxWidth; x++)
        {
          tft.fillWindowPixelTrueSafe(xCache[posX + x], 0, yy, posX + x, ys);
        }
      }
    }
  }
  posX += moveX;
  posY += moveY;
  tft.disableTrueColor();
  tft.fillRect(posX, posY, boxWidth, boxHeight, color565(255, 220, 0));
  tft.enableTrueColor();
  timer++;
  if ((timer & 2047) > 1024)
  {
    tft.scroll(2);
    byte yy = ((tft.height() - 1) * 16 / (byte)(tft.height() / 4)) + 64;
    timer += 80;
    byte gg = (timer & 511) < 256 ? (byte)timer : 255 - (byte)timer;
    timer -= 80;
    uint16_t ys = tft.setWindowLine(0, tft.height() - 2, tft.width() - 1);
    for (byte x = 0; x < tft.width(); x++)
    {
      tft.fillWindowPixelTrue(xCache[x], gg, yy, x, ys);
    }
    ys = tft.setWindowLine(0, tft.height() - 1, tft.width() - 1);
    for (byte x = 0; x < tft.width(); x++)
    {
      tft.fillWindowPixelTrue(xCache[x], gg, yy, x, ys);
   }
 }
#ifndef __AVR_ATmega2560__
  tft.vSync();
#endif
}
