// Mx_Display_Scrollable.h

#ifndef _MX_DISPLAY_SCROLLABLE_h
#define _MX_DISPLAY_SCROLLABLE_h

#include "arduino.h"
#include "core/Mx_Basics.h"

template<typename Mx_Display>
class Mx_Display_Scrollable
{
public:
  uint16_t scrollPos;

  Mx_Display_Scrollable(Mx_Display* display)
  {
    tft = display;
    scrollPos = 0;
  }

  bool begin(bool rotate180)
  {
    tft->begin(rotate180);
    scrollPos = 0;
  }

  bool reset(bool rotate180)
  {
    tft->reset(rotate180);
    scrollPos = 0;
  }

  uint16_t width()
  {
    return tft->width();
  }

  uint16_t height()
  {
    return tft->height();
  }

  void clear(uint16_t color = 0x0000)
  {
    tft->fillRect(0, scrollPos, tft->width(), tft->height() - scrollPos, color);
    tft->fillRect(0, 0, tft->width(), scrollPos, color);
  }

  void drawPixel(int16_t x, int16_t y, uint16_t color)
  {
    if ((uint16_t)y >= tft->height()) return;
    y += scrollPos;
    if ((uint16_t)y >= tft->height()) y -= tft->height();
    tft->drawPixel(x, y, color);
  }

  void drawPixelTrue(int16_t x, int16_t y, uint8_t colorR, uint8_t colorG, uint8_t colorB)
  {
    if ((uint16_t)y >= tft->height()) return;
    y += scrollPos;
    if ((uint16_t)y >= tft->height()) y -= tft->height();
    tft->drawPixelTrue(x, y, colorR, colorG, colorB);
  }

  void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
  {
    bool steep = abs(y2 - y1) > abs(x2 - x1);
    if (steep) { swapInt(x1, y1); swapInt(x2, y2); }
    if (x1 > x2) { swapInt(x1, x2); swapInt(y1, y2); }

    int16_t dx = x2 - x1;
    int16_t dy = abs(y2 - y1);
    int16_t err = dx / 2;
    int16_t ystep = y1 < y2 ? 1 : -1;

    if (steep)
    {
      for (; x1 <= x2; x1++)
      {
        drawPixel(y1, x1, color);
        err -= dy;
        if (err < 0)
        {
          y1 += ystep;
          err += dx;
        }
      }
    }
    else
    {
      for (; x1 <= x2; x1++)
      {
        drawPixel(x1, y1, color);
        err -= dy;
        if (err < 0)
        {
          y1 += ystep;
          err += dx;
        }
      }
    }
  }

  void drawLineTrue(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t colorR, uint8_t colorG, uint8_t colorB)
  {
    bool steep = abs(y2 - y1) > abs(x2 - x1);
    if (steep) { swapInt(x1, y1); swapInt(x2, y2); }
    if (x1 > x2) { swapInt(x1, x2); swapInt(y1, y2); }

    int16_t dx = x2 - x1;
    int16_t dy = abs(y2 - y1);
    int16_t err = dx / 2;
    int16_t ystep = y1 < y2 ? 1 : -1;

    if (steep)
    {
      for (; x1 <= x2; x1++)
      {
        drawPixelTrue(y1, x1, colorR, colorG, colorB);
        err -= dy;
        if (err < 0)
        {
          y1 += ystep;
          err += dx;
        }
      }
    }
    else
    {
      for (; x1 <= x2; x1++)
      {
        drawPixelTrue(x1, y1, colorR, colorG, colorB);
        err -= dy;
        if (err < 0)
        {
          y1 += ystep;
          err += dx;
        }
      }
    }
  }

  void fillRect(int16_t x1, int16_t y1, int16_t width, int16_t height, uint16_t color)
  {
    if (y1 < 0) { height += y1; y1 = 0; }
    if (y1 + height > tft->height()) height = tft->height() - y1;
    if (height <= 0) return;
    tft->fillRect(x1, y1 + scrollPos - tft->height(), width, height, color);
    tft->fillRect(x1, y1 + scrollPos, width, height, color);
  }

  void fillRectTrue(int16_t x1, int16_t y1, int16_t width, int16_t height, uint16_t colorR, uint16_t colorG, uint16_t colorB)
  {
    if (y1 < 0) { height += y1; y1 = 0; }
    if (y1 + height > tft->height()) height = tft->height() - y1;
    if (height <= 0) return;
    tft->fillRectTrue(x1, y1 + scrollPos - tft->height(), width, height, colorR, colorG, colorB);
    tft->fillRectTrue(x1, y1 + scrollPos, width, height, colorR, colorG, colorB);
  }

  uint16_t getScanline()
  {
    return tft->getScanline();
  }

  void vSync(uint16_t line = 260)
  {
    tft->vSync(line);
  }

  void setScrollPosition(uint16_t posY)
  {
    tft->setScrollPosition(posY);
    scrollPos = posY;
  }

  void scroll(int upPixel)
  {
    scrollPos += upPixel;
    if (scrollPos >= tft->height())
    {
      if (scrollPos > 32768) scrollPos += tft->height(); else scrollPos -= tft->height();
    }
    tft->setScrollPosition(scrollPos);
  }

  uint16_t setWindowLine(uint16_t x1, uint16_t y, uint16_t x2)
  {
    y += scrollPos;
    if ((uint16_t)y >= tft->height()) y -= tft->height();
    tft->setWindow(x1, y, x2, tft->height() - 1);
    return y;
  }

  uint16_t setWindowLineSafe(int16_t x1, int16_t y, int16_t x2)
  {
    if ((uint16_t)y >= tft->height()) return;
    if (x1 < 0) x1 = 0;
    if (x2 >= tft->width()) x2 = tft->width() - 1;
    y += scrollPos;
    if ((uint16_t)y >= tft->height()) y -= tft->height();
    tft->setWindow(x1, y, x2, y);
    return y;
  }

  void fillWindowPixel(uint16_t color)
  {
    tft->fillWindowPixel(color);
  }

  void fillWindowPixel(uint16_t color, uint16_t count)
  {
    tft->fillWindow(color, count);
  }

  void fillWindowPixelTrue(uint8_t colorR, uint8_t colorG, uint8_t colorB, uint8_t x, uint8_t y)
  {
    tft->fillWindowPixelTrue(colorR, colorG, colorB, x, y);
  }

  void fillWindowPixelTrueSafe(uint8_t colorR, uint8_t colorG, uint8_t colorB, int16_t x, int16_t y)
  {
    if ((uint16_t)x < tft->width() && (uint16_t)y < tft->height()) tft->fillWindowPixelTrue(colorR, colorG, colorB, x, y);
  }

  void enableTrueColor()
  {
    tft->enableTrueColor();
  }

  void disableTrueColor()
  {
    tft->disableTrueColor();
  }

private:
  Mx_Display* tft;
};

#endif
