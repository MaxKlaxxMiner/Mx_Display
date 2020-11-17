
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
#define DisplaySerialName tft;
#define DisplayWidth 320
#define DisplayHeight 240

#define Cmd(cmd) DisplaySerialPort.write((uint8_t)(cmd));

#define Cmd1b(cmd, val1) DisplaySerialPort.write((uint8_t)(cmd)); DisplaySerialPort.write((uint8_t)(val1));

#define Cmd1w(cmd, val1) DisplaySerialPort.write((uint8_t)(cmd)); \
                         DisplaySerialPort.write((uint8_t)(val1)); DisplaySerialPort.write((uint8_t)((val1) >> 8));

#define Cmd4w(cmd, val1, val2, val3, val4) DisplaySerialPort.write((uint8_t)(cmd)); \
                                           DisplaySerialPort.write((uint8_t)(val1)); DisplaySerialPort.write((uint8_t)((val1) >> 8)); \
                                           DisplaySerialPort.write((uint8_t)(val2)); DisplaySerialPort.write((uint8_t)((val2) >> 8)); \
                                           DisplaySerialPort.write((uint8_t)(val3)); DisplaySerialPort.write((uint8_t)((val3) >> 8)); \
                                           DisplaySerialPort.write((uint8_t)(val4)); DisplaySerialPort.write((uint8_t)((val4) >> 8));

#define Cmd5w(cmd, val1, val2, val3, val4, val5) DisplaySerialPort.write((uint8_t)(cmd)); \
                                                 DisplaySerialPort.write((uint8_t)(val1)); DisplaySerialPort.write((uint8_t)((val1) >> 8)); \
                                                 DisplaySerialPort.write((uint8_t)(val2)); DisplaySerialPort.write((uint8_t)((val2) >> 8)); \
                                                 DisplaySerialPort.write((uint8_t)(val3)); DisplaySerialPort.write((uint8_t)((val3) >> 8)); \
                                                 DisplaySerialPort.write((uint8_t)(val4)); DisplaySerialPort.write((uint8_t)((val4) >> 8)); \
                                                 DisplaySerialPort.write((uint8_t)(val5)); DisplaySerialPort.write((uint8_t)((val5) >> 8));

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

Mx_Display_Serial_Adafruit DisplaySerialName;

#endif _MX_DISPLAY_SERIAL_ADA_h
