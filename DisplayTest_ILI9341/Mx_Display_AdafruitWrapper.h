#ifndef _MX_DISPLAY_ADAFRUITWRAPPER_h
#define _MX_DISPLAY_ADAFRUITWRAPPER_h

#include "arduino.h"
#include <Adafruit_GFX.h>

template<typename Mx_Display>
class Mx_Display_AdafruitWrapper : public Adafruit_GFX
{
public:
  Mx_Display_AdafruitWrapper(Mx_Display* display) : Adafruit_GFX(tft->width(), tft->height())
  {
    tft = display;
  }

  void drawPixel(int16_t x, int16_t y, uint16_t color)
  {
    int16_t t;
    switch (rotation)
    {
      case 1: t = x; x = WIDTH - 1 - y; y = t; break;
      case 2: x = WIDTH - 1 - x; y = HEIGHT - 1 - y; break;
      case 3: t = x; x = y; y = HEIGHT - 1 - t; break;
    }
    tft->drawPixel(x, y, color);
  }

  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
  {
    int16_t t;
    switch (rotation)
    {
      case 1:
        t = x0; x0 = WIDTH - 1 - y0; y0 = t;
        t = x1; x1 = WIDTH - 1 - y1; y1 = t;
        break;
      case 2:
        x0 = WIDTH - 1 - x0; y0 = HEIGHT - 1 - y0;
        x1 = WIDTH - 1 - x1; y1 = HEIGHT - 1 - y1;
        break;
      case 3:
        t = x0; x0 = y0; y0 = HEIGHT - 1 - t;
        t = x1; x1 = y1; y1 = HEIGHT - 1 - t;
        break;
    }
    tft->drawLine(x0, y0, x1, y1, color);
  }

  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
  {
    int16_t t;
    switch (rotation)
    {
      case 1:
        t = x; x = WIDTH - h - y; y = t;
        t = w; w = h; h = t;
        break;
      case 2:
        x = WIDTH - w - x; y = HEIGHT - h - y;
        break;
      case 3:
        t = x; x = y; y = HEIGHT - w - t;
        t = w; w = h; h = t;
        break;
    }
    tft->fillRect(x, y, w, h, color);
  }

  void fillScreen(uint16_t color)
  {
    tft->clear(color);
  }

  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
  {
    fillRect(x, y, 1, h, color);
  }

  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
  {
    fillRect(x, y, w, 1, color);
  }

  void writePixel(int16_t x, int16_t y, uint16_t color)
  {
    drawPixel(x, y, color);
  }

  void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
  {
    fillRect(x, y, w, h, color);
  }

  void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
  {
    drawFastVLine(x, y, h, color);
  }

  void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
  {
    drawFastHLine(x, y, w, color);
  }

  void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
  {
    drawLine(x0, y0, x1, y1, color);
  }

private:
  Mx_Display* tft;
};

#endif
