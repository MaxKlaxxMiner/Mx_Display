
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
  /// fills the entire screen with one colour
  /// </summary>
  CmdFillScreen = 0x01,
  /// <summary>
  /// fast drawing of a horizontal line [int16_t x, int16_t y, int16_t w, uint16_t color]
  /// </summary>
  CmdFastHLine = 0x02,
  /// <summary>
  /// fast drawing of a vertical line [int16_t x, int16_t y, int16_t h, uint16_t color]
  /// </summary>
  CmdFastVLine = 0x03,
};

#define DisplaySerialPort Serial
#define DisplaySerialName tft;
#define DisplayWidth 320
#define DisplayHeight 240

#define Cmd(cmd) DisplaySerialPort.write((uint8_t)(cmd));

#define Cmd1w(cmd, val1) DisplaySerialPort.write((uint8_t)(cmd)); \
                         DisplaySerialPort.write((uint8_t)(val1)); DisplaySerialPort.write((uint8_t)((val1) >> 8));

#define Cmd4w(cmd, val1, val2, val3, val4) DisplaySerialPort.write((uint8_t)(cmd)); \
                                           DisplaySerialPort.write((uint8_t)(val1)); DisplaySerialPort.write((uint8_t)((val1) >> 8)); \
                                           DisplaySerialPort.write((uint8_t)(val2)); DisplaySerialPort.write((uint8_t)((val2) >> 8)); \
                                           DisplaySerialPort.write((uint8_t)(val3)); DisplaySerialPort.write((uint8_t)((val3) >> 8)); \
                                           DisplaySerialPort.write((uint8_t)(val4)); DisplaySerialPort.write((uint8_t)((val4) >> 8));

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

private:
  int16_t _width = DisplayWidth;
  int16_t _height = DisplayHeight;
};

Mx_Display_Serial_Adafruit DisplaySerialName;

#endif _MX_DISPLAY_SERIAL_ADA_h
