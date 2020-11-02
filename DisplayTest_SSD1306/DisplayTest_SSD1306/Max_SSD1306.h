#ifndef _MAX_SSD1306_h
#define _MAX_SSD1306_h

#include "arduino.h"
#include "TwoMaxWire.h"

#define WIRE_MAX 64

#define WIDTH   128 // display width, in pixels
#define HEIGHT   64 // display height, in pixels
#define BUFFER_BYTES (WIDTH * HEIGHT + 7) / 8

#define SSD1306_MEMORYMODE          0x20 ///< See datasheet
#define SSD1306_COLUMNADDR          0x21 ///< See datasheet
#define SSD1306_PAGEADDR            0x22 ///< See datasheet
#define SSD1306_SETCONTRAST         0x81 ///< See datasheet
#define SSD1306_CHARGEPUMP          0x8D ///< See datasheet
#define SSD1306_SEGREMAP            0xA0 ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON        0xA5 ///< Not currently used
#define SSD1306_NORMALDISPLAY       0xA6 ///< See datasheet
#define SSD1306_INVERTDISPLAY       0xA7 ///< See datasheet
#define SSD1306_SETMULTIPLEX        0xA8 ///< See datasheet
#define SSD1306_DISPLAYOFF          0xAE ///< See datasheet
#define SSD1306_DISPLAYON           0xAF ///< See datasheet
#define SSD1306_COMSCANINC          0xC0 ///< Not currently used
#define SSD1306_COMSCANDEC          0xC8 ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET    0xD3 ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5 ///< See datasheet
#define SSD1306_SETPRECHARGE        0xD9 ///< See datasheet
#define SSD1306_SETCOMPINS          0xDA ///< See datasheet
#define SSD1306_SETVCOMDETECT       0xDB ///< See datasheet

#define SSD1306_SETLOWCOLUMN        0x00 ///< Not currently used
#define SSD1306_SETHIGHCOLUMN       0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE        0x40 ///< See datasheet

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26 ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27 ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 ///< Set scroll range

class Max_SSD1306 : public Print
{
public:
  Max_SSD1306(TwoMaxWire* twi = &MaxWire);

  ~Max_SSD1306(void);

  bool begin(byte i2caddr = 0x3c, uint32_t wireClock = 810000UL);
  void display(void);
  void clearDisplay(byte color = 0);
  void invertDisplay(bool invert);
  void dim(bool dim);
  void pump(bool chargePump);
  void drawPixel(int x, int y, byte color);

  using Print::write;
  virtual size_t write(byte);

  void setTextSize(byte size);
  void setTextSize(byte sizeX, byte sizeY);
  void setTextColor(byte c);
  void setCursor(byte x, byte y);

  void drawChar(byte x, byte y, byte c, byte color);
  void drawChar(byte x, byte y, byte c, byte color, byte size_x, byte size_y);

private:
  void ssd1306_command(byte c);
  void ssd1306_command(byte c, byte v);
  void ssd1306_commandList(const byte* c, byte n);

  TwoMaxWire* wire;
  byte* buffer;
  byte  i2caddr;

  byte  cursor_x, cursor_y;
  byte  textcolor;
  byte  textsize_x, textsize_y, textsize_w;
  bool  wrap;
};

#endif
