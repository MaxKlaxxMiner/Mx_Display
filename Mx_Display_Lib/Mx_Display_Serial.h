
#ifndef _MX_DISPLAY_SERIAL_h
#define _MX_DISPLAY_SERIAL_h

#include "arduino.h"
#include <Adafruit_GFX.h>

enum DisplayCmdType
{
  /// <summary>
  /// [1] - get display info
  /// </summary>
  GetInfo = 0x00,
  /// <summary>
  /// [1 + 2] - select 16-Bit color (red: 5-Bit, green: 6-Bit, blue: 5-Bit)
  /// </summary>
  SelectColor565 = 0x01,
  /// <summary>
  /// [1 + 3] - select 24-Bit true-color (red: 8-Bit, green: 8-Bit, blue: 8-Bit)
  /// </summary>
  SelectColorTrue = 0x02,
  /// <summary>
  /// [1 + 2 + 2] - draw pixel with current color
  /// </summary>
  DrawPixel = 0x03,
  /// <summary>
  /// [1 + 2 + 2 + 2] - draw horizontal line with current color
  /// </summary>
  DrawHLine = 0x04,
  /// <summary>
  /// [1 + 2 + 2 + 2] - draw vertical line with current color
  /// </summary>
  DrawVLine = 0x05,
  /// <summary>
  /// [1 + 2 + 2 + 2 + 2] - draw normal line with current color
  /// </summary>
  DrawLine = 0x06,
};

#define DisplaySerialPort Serial
#define DisplaySerialName tft;

class Mx_Display_Serial_Adafruit : public Adafruit_GFX
{
public:
  Mx_Display_Serial_Adafruit() : Adafruit_GFX(320, 240)
  {
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

    setColor(color);
    setPixel(x, y);
  }

  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
  {
    setColor(color);
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
    setLine(x0, y0, x1, y1);
  }

  void fillScreen(uint16_t color)
  {
    setColor(color);
    for (int16_t y = 0; y < HEIGHT; y++)
    {
      setHLine(0, y, WIDTH);
    }
  }

  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
  {
    setColor(color);
    if (h < 0)  // Convert negative heights to positive equivalent
    {
      h *= -1;
      y -= h - 1;
      if (y < 0)
      {
        h += y;
        y = 0;
      }
    }

    // Edge rejection (no-draw if totally off canvas)
    if ((x < 0) || (x >= width()) || (y >= height()) || ((y + h - 1) < 0)) return;

    if (y < 0) // Clip top
    {
      h += y;
      y = 0;
    }
    if (y + h > height()) // Clip bottom
    {
      h = height() - y;
    }

    if (getRotation() == 0)
    {
      setVLine(x, y, h);
    }
    else if (getRotation() == 1)
    {
      int16_t t = x;
      x = WIDTH - 1 - y;
      y = t;
      x -= h - 1;
      setHLine(x, y, h);
    }
    else if (getRotation() == 2)
    {
      x = WIDTH - 1 - x;
      y = HEIGHT - 1 - y;

      y -= h - 1;
      setVLine(x, y, h);
    }
    else if (getRotation() == 3)
    {
      int16_t t = x;
      x = y;
      y = HEIGHT - 1 - t;
      setHLine(x, y, h);
    }
  }

  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
  {
    setColor(color);
    if (w < 0)
    { // Convert negative widths to positive equivalent
      w *= -1;
      x -= w - 1;
      if (x < 0)
      {
        w += x;
        x = 0;
      }
    }

    // Edge rejection (no-draw if totally off canvas)
    if ((y < 0) || (y >= height()) || (x >= width()) || ((x + w - 1) < 0))
    {
      return;
    }

    if (x < 0) // Clip left
    {
      w += x;
      x = 0;
    }
    if (x + w >= width()) // Clip right
    { 
      w = width() - x;
    }

    if (getRotation() == 0)
    {
      setHLine(x, y, w);
    }
    else if (getRotation() == 1)
    {
      int16_t t = x;
      x = WIDTH - 1 - y;
      y = t;
      setVLine(x, y, w);
    }
    else if (getRotation() == 2)
    {
      x = WIDTH - 1 - x;
      y = HEIGHT - 1 - y;

      x -= w - 1;
      setHLine(x, y, w);
    }
    else if (getRotation() == 3)
    {
      int16_t t = x;
      x = y;
      y = HEIGHT - 1 - t;
      y -= w - 1;
      setVLine(x, y, w);
    }
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
  void setColor(uint16_t c)
  {
    DisplaySerialPort.write((uint8_t)SelectColor565);
    DisplaySerialPort.write((uint8_t)c);
    DisplaySerialPort.write((uint8_t)(c >> 8));
  }
  void setPixel(int16_t x, int16_t y)
  {
    DisplaySerialPort.write((uint8_t)DrawPixel);
    DisplaySerialPort.write((uint8_t)(uint16_t)x);
    DisplaySerialPort.write((uint8_t)((uint16_t)x >> 8));
    DisplaySerialPort.write((uint8_t)(uint16_t)y);
    DisplaySerialPort.write((uint8_t)((uint16_t)y >> 8));
  }
  void setHLine(int16_t x, int16_t y, int16_t len)
  {
    DisplaySerialPort.write((uint8_t)DrawHLine);
    DisplaySerialPort.write((uint8_t)(uint16_t)x);
    DisplaySerialPort.write((uint8_t)((uint16_t)x >> 8));
    DisplaySerialPort.write((uint8_t)(uint16_t)y);
    DisplaySerialPort.write((uint8_t)((uint16_t)y >> 8));
    DisplaySerialPort.write((uint8_t)(uint16_t)len);
    DisplaySerialPort.write((uint8_t)((uint16_t)len >> 8));
  }
  void setVLine(int16_t x, int16_t y, int16_t len)
  {
    DisplaySerialPort.write((uint8_t)DrawVLine);
    DisplaySerialPort.write((uint8_t)(uint16_t)x);
    DisplaySerialPort.write((uint8_t)((uint16_t)x >> 8));
    DisplaySerialPort.write((uint8_t)(uint16_t)y);
    DisplaySerialPort.write((uint8_t)((uint16_t)y >> 8));
    DisplaySerialPort.write((uint8_t)(uint16_t)len);
    DisplaySerialPort.write((uint8_t)((uint16_t)len >> 8));
  }
  void setLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
  {
    DisplaySerialPort.write((uint8_t)DrawLine);
    DisplaySerialPort.write((uint8_t)(uint16_t)x1);
    DisplaySerialPort.write((uint8_t)((uint16_t)x1 >> 8));
    DisplaySerialPort.write((uint8_t)(uint16_t)y1);
    DisplaySerialPort.write((uint8_t)((uint16_t)y1 >> 8));
    DisplaySerialPort.write((uint8_t)(uint16_t)x2);
    DisplaySerialPort.write((uint8_t)((uint16_t)x2 >> 8));
    DisplaySerialPort.write((uint8_t)(uint16_t)y2);
    DisplaySerialPort.write((uint8_t)((uint16_t)y2 >> 8));
  }
};

Mx_Display_Serial_Adafruit DisplaySerialName;

#endif _MX_DISPLAY_SERIAL_h
