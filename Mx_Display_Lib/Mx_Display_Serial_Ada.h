
#ifndef _MX_DISPLAY_SERIAL_ADA_h
#define _MX_DISPLAY_SERIAL_ADA_h

#include "arduino.h"
#include <Adafruit_GFX.h>

/// <summary>
/// command-types
/// </summary>
enum CmdAdafruitType
{
  /// <summary>
  /// empty command (no operation)
  /// </summary>
  CmdNop = 0x00,
  /// <summary>
  /// fills the entire screen with one colour
  /// [uint16_t color]
  /// </summary>
  CmdFillScreen,
  /// <summary>
  /// draw a pixel :)
  /// [int16_t x, int16_t y, uint16_t color]
  /// </summary>
  CmdDrawPixel,
  /// <summary>
  /// fast drawing of a horizontal line
  /// [int16_t x, int16_t y, int16_t w, uint16_t color]
  /// </summary>
  CmdFastHLine,
  /// <summary>
  /// fast drawing of a vertical line
  /// [int16_t x, int16_t y, int16_t h, uint16_t color]
  /// </summary>
  CmdFastVLine,
  /// <summary>
  /// drawing normal line
  /// [int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color]
  /// </summary>
  CmdDrawLine,
  /// <summary>
  /// draw a rectangle with no fill color
  /// [int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color]
  /// </summary>
  CmdDrawRect,
  /// <summary>
  /// fill a rectangle completely with one color
  /// [int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color]
  /// </summary>
  CmdFillRect,
  /// <summary>
  /// draw a circle outline
  /// [int16_t x, int16_t y, int16_t r, uint16_t color]
  /// </summary>
  CmdDrawCircle,
  /// <summary>
  /// draw a circle with filled color
  /// [int16_t x, int16_t y, int16_t r, uint16_t color]
  /// </summary>
  CmdFillCircle,
  /// <summary>
  /// draw a triangle with no fill color
  /// [int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color]
  /// </summary>
  CmdDrawTriangle,
  /// <summary>
  /// draw a triangle with filled color
  /// [int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color]
  /// </summary>
  CmdFillTriangle,
  /// <summary>
  /// draw a rounded rectangle with no fill color
  /// [int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color]
  /// </summary>
  CmdDrawRoundRect,
  /// <summary>
  /// draw a rounded rectangle with fill color
  /// [int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color]
  /// </summary>
  CmdFillRoundRect,
  /// <summary>
  /// set display rotation (0-3)
  /// [uint8_t rotation]
  /// </summary>
  CmdSetRotation,
  /// <summary>
  /// set backbuffer (0-255)
  /// [uint8_t index]
  /// </summary>
  CmdSetBackBuffer,
  /// <summary>
  /// copy current display to another backbuffer (or frontbuffer)
  /// [uint8_t index]
  /// </summary>
  CmdCopyToBackbuffer,
};

#define DisplaySerialPort Serial
//#define DisplaySerialName tft;
#define DisplayWidth 320
#define DisplayHeight 240

#define Cmd(cmd) DisplaySerialPort.write((uint8_t)(cmd));
#define Cmd1b(cmd, val1) Cmd(cmd); DisplaySerialPort.write((uint8_t)(val1));
#define Val1w(val) DisplaySerialPort.write((uint8_t)(val)); DisplaySerialPort.write((uint8_t)((val) >> 8));
#define Cmd1w(cmd, val1) Cmd(cmd); Val1w(val1);
#define Cmd2w(cmd, val1, val2) Cmd1w(cmd, val1); Val1w(val2);
#define Cmd3w(cmd, val1, val2, val3) Cmd2w(cmd, val1, val2); Val1w(val3);
#define Cmd4w(cmd, val1, val2, val3, val4) Cmd3w(cmd, val1, val2, val3); Val1w(val4);
#define Cmd5w(cmd, val1, val2, val3, val4, val5) Cmd4w(cmd, val1, val2, val3, val4); Val1w(val5);
#define Cmd6w(cmd, val1, val2, val3, val4, val5, val6) Cmd5w(cmd, val1, val2, val3, val4, val5); Val1w(val6);
#define Cmd7w(cmd, val1, val2, val3, val4, val5, val6, val7) Cmd6w(cmd, val1, val2, val3, val4, val5, val6); Val1w(val7);

#ifdef Mx_Display_Reference
class Mx_Display_Serial_Adafruit : public Adafruit_GFX
{
public:
  Mx_Display_Serial_Adafruit(byte dummy) : Adafruit_GFX(DisplayWidth, DisplayHeight)
  {
  }

  void drawPixel(int16_t x, int16_t y, uint16_t color)
  {
    Cmd3w(CmdDrawPixel, x, y, color);
  }

  void setBackbuffer(uint8_t b)
  {
    Cmd1b(CmdSetBackBuffer, b);
  }

  void copyToBackbuffer(uint8_t b)
  {
    Cmd1b(CmdCopyToBackbuffer, b);
  }
};
#else
class Mx_Display_Serial_Adafruit
{
public:
  /// <summary>
  /// width of the screen
  /// </summary>
  /// <returns>width in pixels</returns>
  int16_t width()
  {
    return _width;
  }

  /// <summary>
  /// height of the screen
  /// </summary>
  /// <returns>height in pixels</returns>
  int16_t height()
  {
    return _height;
  }

  /// <summary>
  /// fills the entire screen with one colour
  /// </summary>
  /// <param name="color">fill-color</param>
  void fillScreen(uint16_t color)
  {
    Cmd1w(CmdFillScreen, color);
  }

  /// <summary>
  /// fast drawing of a horizontal line
  /// </summary>
  /// <param name="x">start x-position</param>
  /// <param name="y">start y-position</param>
  /// <param name="w">width / length</param>
  /// <param name="color">line-color</param>
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
  {
    Cmd4w(CmdFastHLine, x, y, w, color);
  }

  /// <summary>
  /// fast drawing of a vertical line
  /// </summary>
  /// <param name="x">start x-position</param>
  /// <param name="y">start y-position</param>
  /// <param name="h">height / length</param>
  /// <param name="color">line-color</param>
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
  {
    Cmd4w(CmdFastVLine, x, y, h, color);
  }

  /// <summary>
  /// drawing normal line
  /// </summary>
  /// <param name="x1">start x-position</param>
  /// <param name="y1">start y-position</param>
  /// <param name="x2">end x-position</param>
  /// <param name="y2">end y-position</param>
  /// <param name="color">line-color</param>
  void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
  {
    Cmd5w(CmdDrawLine, x1, y1, x2, y2, color);
  }

  /// <summary>
  /// draw a rectangle with no fill color
  /// </summary>
  /// <param name="x">start x-position</param>
  /// <param name="y">start y-position</param>
  /// <param name="w">rectangle width</param>
  /// <param name="h">rectangle height</param>
  /// <param name="color">border-color</param>
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
  {
    Cmd5w(CmdDrawRect, x, y, w, h, color);
  }

  /// <summary>
  /// fill a rectangle completely with one color
  /// </summary>
  /// <param name="x">start x-position</param>
  /// <param name="y">start y-position</param>
  /// <param name="w">rectangle width</param>
  /// <param name="h">rectangle height</param>
  /// <param name="color">fill-color</param>
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
  {
    Cmd5w(CmdFillRect, x, y, w, h, color);
  }

  /// <summary>
  /// draw a circle outline
  /// </summary>
  /// <param name="x">center x-position</param>
  /// <param name="y">center y-position</param>
  /// <param name="r">radius</param>
  /// <param name="color">border-color</param>
  void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color)
  {
    Cmd4w(CmdDrawCircle, x, y, r, color);
  }

  /// <summary>
  /// draw a circle with filled color
  /// </summary>
  /// <param name="x">center x-position</param>
  /// <param name="y">center y-position</param>
  /// <param name="r">radius</param>
  /// <param name="color">fill-color</param>
  void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color)
  {
    Cmd4w(CmdFillCircle, x, y, r, color);
  }

  /// <summary>
  /// draw a triangle with no fill color
  /// </summary>
  /// <param name="x1">first vertex x-position</param>
  /// <param name="y1">first vertex y-position</param>
  /// <param name="x2">second vertex x-position</param>
  /// <param name="y2">second vertex y-position</param>
  /// <param name="x3">third vertex x-position</param>
  /// <param name="y3">third vertex y-position</param>
  /// <param name="color">border-color</param>
  void drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color)
  {
    Cmd7w(CmdDrawTriangle, x1, y1, x2, y2, x3, y3, color);
  }

  /// <summary>
  /// draw a triangle with filled color
  /// </summary>
  /// <param name="x1">first vertex x-position</param>
  /// <param name="y1">first vertex y-position</param>
  /// <param name="x2">second vertex x-position</param>
  /// <param name="y2">second vertex y-position</param>
  /// <param name="x3">third vertex x-position</param>
  /// <param name="y3">third vertex y-position</param>
  /// <param name="color">fill-color</param>
  void fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color)
  {
    Cmd7w(CmdFillTriangle, x1, y1, x2, y2, x3, y3, color);
  }

  /// <summary>
  /// draw a rounded rectangle with no fill color
  /// </summary>
  /// <param name="x">start x-position</param>
  /// <param name="y">start y-position</param>
  /// <param name="w">width</param>
  /// <param name="h">height</param>
  /// <param name="r">border-radius</param>
  /// <param name="color">border-color</param>
  void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
  {
    Cmd6w(CmdDrawRoundRect, x, y, w, h, r, color);
  }

  /// <summary>
  /// draw a rounded rectangle with fill color
  /// </summary>
  /// <param name="x">start x-position</param>
  /// <param name="y">start y-position</param>
  /// <param name="w">width</param>
  /// <param name="h">height</param>
  /// <param name="r">border-radius</param>
  /// <param name="color">fill-color</param>
  void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
  {
    Cmd6w(CmdFillRoundRect, x, y, w, h, r, color);
  }

  /// <summary>
  /// set display rotation
  /// </summary>
  /// <param name="r">rotation value (0-3)</param>
  void setRotation(uint8_t r)
  {
    Cmd1b(CmdSetRotation, r);
    if (r & 1)
    {
      _width = DisplayHeight;
      _height = DisplayWidth;
    }
    else
    {
      _width = DisplayWidth;
      _height = DisplayHeight;
    }
  }

  /// <summary>
  /// set the drawing backbuffer
  /// </summary>
  /// <param name="b">index of the backbuffer</param>
  void setBackbuffer(uint8_t b)
  {
    Cmd1b(CmdSetBackBuffer, b);
  }

  /// <summary>
  /// copy current buffer to target backbuffer
  /// </summary>
  /// <param name="b">index of the target backbuffer</param>
  void copyToBackbuffer(uint8_t b)
  {
    Cmd1b(CmdCopyToBackbuffer, b);
  }

private:
  int16_t _width = DisplayWidth;
  int16_t _height = DisplayHeight;
};
#endif

#ifdef Mx_Display_Reference
Mx_Display_Serial_Adafruit tft(0);
#else
//Mx_Display_Serial_Adafruit DisplaySerialName;
Mx_Display_Serial_Adafruit tft;
#endif

#endif _MX_DISPLAY_SERIAL_ADA_h
