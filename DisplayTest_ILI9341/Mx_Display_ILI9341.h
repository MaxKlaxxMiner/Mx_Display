#ifndef _Mx_Display_ILI9341_h
#define _Mx_Display_ILI9341_h

#include "arduino.h"
#include "core/Mx_Basics.h"
#include "core/Mx_Display_ILI9341_Consts.h"

#define MxMethodDefinitions                                                                        \
  bool begin(bool rotate180);                                                                      \
  bool reset(bool rotate180);                                                                      \
  uint16_t width();                                                                                \
  uint16_t height();                                                                               \
  void clear(uint16_t color = 0x0000);                                                             \
  void clearTrue(uint8_t colorR = 0, uint8_t colorG = 0, uint8_t colorB = 0);                      \
  void drawPixel(int16_t x, int16_t y, uint16_t color);                                            \
  void drawPixelTrue(int16_t x, int16_t y, uint8_t colorR, uint8_t colorG, uint8_t colorB);        \
  void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);                   \
  void drawLineTrue(int16_t x1, int16_t y1, int16_t x2, int16_t y2,                                \
                    uint8_t colorR, uint8_t colorG, uint8_t colorB);                               \
  void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color);                                \
  void drawCircleTrue(int16_t x, int16_t y, int16_t r,                                             \
                      uint8_t colorR, uint8_t colorG, uint8_t colorB);                             \
  void drawCircleHelper(int16_t x, int16_t y, int16_t r, uint8_t corner, uint16_t color);          \
  void drawCircleHelperTrue(int16_t x, int16_t y, int16_t r, uint8_t corner,                       \
                            uint8_t colorR, uint8_t colorG, uint8_t colorB);                       \
  void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color);                                \
  void fillCircleTrue(int16_t x, int16_t y, int16_t r,                                             \
                      uint8_t colorR, uint8_t colorG, uint8_t colorB);                             \
  void fillCircleHelper(int16_t x, int16_t y, int16_t r, uint8_t corner, int16_t delta, uint16_t color); \
  void fillCircleHelperTrue(int16_t x, int16_t y, int16_t r, uint8_t corner, int16_t delta,        \
                            uint8_t colorR, uint8_t colorG, uint8_t colorB);                       \
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);                       \
  void drawRectTrue(int16_t x, int16_t y, int16_t w, int16_t h,                                    \
                    uint8_t colorR, uint8_t colorG, uint8_t colorB);                               \
  void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);       \
  void drawRoundRectTrue(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,                    \
                         uint8_t colorR, uint8_t colorG, uint8_t colorB);                          \
  void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);       \
  void fillRoundRectTrue(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,                    \
                         uint8_t colorR, uint8_t colorG, uint8_t colorB);                          \
  void drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color); \
  void drawTriangleTrue(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3,    \
                        uint8_t colorR, uint8_t colorG, uint8_t colorB);                           \
  void fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color); \
  void fillTriangleTrue(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3,    \
                        uint8_t colorR, uint8_t colorG, uint8_t colorB);                           \
  void drawLineV(int16_t x, int16_t y, int16_t height, uint16_t color);                            \
  void drawLineVTrue(int16_t x, int16_t y, int16_t height,                                         \
                     uint8_t colorR, uint8_t colorG, uint8_t colorB);                              \
  void drawLineH(int16_t x, int16_t y, int16_t width, uint16_t color);                             \
  void drawLineHTrue(int16_t x, int16_t y, int16_t width,                                          \
                     uint8_t colorR, uint8_t colorG, uint8_t colorB);                              \
  void fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);              \
  void fillRectTrue(int16_t x , int16_t y , int16_t width, int16_t height,                         \
                    uint8_t colorR, uint8_t colorG, uint8_t colorB);                               \
  void enableTrueColor();                                                                          \
  void disableTrueColor();                                                                         \
                                                                                                   \
  void vSync(uint16_t line = 260);                                                                 \
  uint16_t getScanline();                                                                          \
  void setScrollPosition(uint16_t posY);                                                           \
  void setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);                              \
  void fillWindow(uint16_t color, uint16_t len);                                                   \
  void fillWindowPixel(uint16_t color);                                                            \
  void fillWindowPixelTrue(uint8_t colorR, uint8_t colorG, uint8_t colorB, uint8_t x, uint8_t y);  \
  void fillWindowLineTrue(uint8_t colorR, uint8_t colorG, uint8_t colorB,                          \
                          uint8_t x, uint8_t y, bool horizontal, uint16_t len);                    \
                                                                                                   \
private:                                                                                           \
  void setWriteDir();                                                                              \
  void setReadDir();                                                                               \
  void write8(uint8_t val);                                                                        \
  void write8inline(uint8_t val);                                                                  \
  uint8_t read8();                                                                                 \
  void command(uint8_t cmd);                                                                       \
  void command(uint8_t cmd, uint8_t val);                                                          \
  void command(uint8_t cmd, uint8_t val1, uint8_t val2);                                           \
  void command16(uint8_t cmd, uint16_t val1, uint16_t val2);                                       \
  void rdIdle();                                                                                   \
  void rdActive();                                                                                 \
  void wrIdle();                                                                                   \
  void wrActive();                                                                                 \
  void cdData();                                                                                   \
  void cdCommand();                                                                                \
  void csIdle();                                                                                   \
  void csActive();                                                                                 \
  void drawLineSafe(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);               \
  void drawLineTrueSafe(int16_t x1, int16_t y1, int16_t x2, int16_t y2,                            \
                        uint8_t colorR, uint8_t colorG, uint8_t colorB);

class Mx_DisplayV_ILI9341_Reference
{
public:
  Mx_DisplayV_ILI9341_Reference(bool rotate180);
  Mx_DisplayV_ILI9341_Reference(bool rotate180, uint8_t rdPin, uint8_t wrPin, uint8_t cdPin, uint8_t csPin, uint8_t rsPin,
                                uint8_t data0Pin, uint8_t data1Pin, uint8_t data2Pin, uint8_t data3Pin,
                                uint8_t data4Pin, uint8_t data5Pin, uint8_t data6Pin, uint8_t data7Pin);
  MxMethodDefinitions;

  uint8_t _rdPin, _wrPin, _cdPin, _csPin, _rsPin;
  uint8_t _data0Pin, _data1Pin, _data2Pin, _data3Pin, _data4Pin, _data5Pin, _data6Pin, _data7Pin;
};

class Mx_DisplayH_ILI9341_Reference
{
public:
  Mx_DisplayH_ILI9341_Reference(bool rotate180);
  Mx_DisplayH_ILI9341_Reference(bool rotate180, uint8_t rdPin, uint8_t wrPin, uint8_t cdPin, uint8_t csPin, uint8_t rsPin,
                                uint8_t data0Pin, uint8_t data1Pin, uint8_t data2Pin, uint8_t data3Pin,
                                uint8_t data4Pin, uint8_t data5Pin, uint8_t data6Pin, uint8_t data7Pin);
  MxMethodDefinitions;

  uint8_t _rdPin, _wrPin, _cdPin, _csPin, _rsPin;
  uint8_t _data0Pin, _data1Pin, _data2Pin, _data3Pin, _data4Pin, _data5Pin, _data6Pin, _data7Pin;
};


// --- Config: Basic ---

class Mx_DisplayV_ILI9341_CompactMinimal
{
public:
  Mx_DisplayV_ILI9341_CompactMinimal(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Compact
{
public:
  Mx_DisplayV_ILI9341_Compact(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Fast
{
public:
  Mx_DisplayV_ILI9341_Fast(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Fastest
{
public:
  Mx_DisplayV_ILI9341_Fastest(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_CompactMinimal
{
public:
  Mx_DisplayH_ILI9341_CompactMinimal(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Compact
{
public:
  Mx_DisplayH_ILI9341_Compact(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Fast
{
public:
  Mx_DisplayH_ILI9341_Fast(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Fastest
{
public:
  Mx_DisplayH_ILI9341_Fastest(bool rotate180);
  MxMethodDefinitions;
};


// --- Config: NoSerial ---

class Mx_DisplayV_ILI9341_CompactMinimal_NoSerial
{
public:
  Mx_DisplayV_ILI9341_CompactMinimal_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_MOD_CompactMinimal_NoSerial
{
public:
  Mx_DisplayV_ILI9341_MOD_CompactMinimal_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Compact_NoSerial
{
public:
  Mx_DisplayV_ILI9341_Compact_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_MOD_Compact_NoSerial
{
public:
  Mx_DisplayV_ILI9341_MOD_Compact_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Fast_NoSerial
{
public:
  Mx_DisplayV_ILI9341_Fast_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_MOD_Fast_NoSerial
{
public:
  Mx_DisplayV_ILI9341_MOD_Fast_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Fastest_NoSerial
{
public:
  Mx_DisplayV_ILI9341_Fastest_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_MOD_Fastest_NoSerial
{
public:
  Mx_DisplayV_ILI9341_MOD_Fastest_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_CompactMinimal_NoSerial
{
public:
  Mx_DisplayH_ILI9341_CompactMinimal_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_MOD_CompactMinimal_NoSerial
{
public:
  Mx_DisplayH_ILI9341_MOD_CompactMinimal_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Compact_NoSerial
{
public:
  Mx_DisplayH_ILI9341_Compact_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_MOD_Compact_NoSerial
{
public:
  Mx_DisplayH_ILI9341_MOD_Compact_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Fast_NoSerial
{
public:
  Mx_DisplayH_ILI9341_Fast_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_MOD_Fast_NoSerial
{
public:
  Mx_DisplayH_ILI9341_MOD_Fast_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Fastest_NoSerial
{
public:
  Mx_DisplayH_ILI9341_Fastest_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_MOD_Fastest_NoSerial
{
public:
  Mx_DisplayH_ILI9341_MOD_Fastest_NoSerial(bool rotate180);
  MxMethodDefinitions;
};


// --- Config: NoSd ---

class Mx_DisplayV_ILI9341_CompactMinimal_NoSd
{
public:
  Mx_DisplayV_ILI9341_CompactMinimal_NoSd(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Compact_NoSd
{
public:
  Mx_DisplayV_ILI9341_Compact_NoSd(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Fast_NoSd
{
public:
  Mx_DisplayV_ILI9341_Fast_NoSd(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Fastest_NoSd
{
public:
  Mx_DisplayV_ILI9341_Fastest_NoSd(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_CompactMinimal_NoSd
{
public:
  Mx_DisplayH_ILI9341_CompactMinimal_NoSd(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Compact_NoSd
{
public:
  Mx_DisplayH_ILI9341_Compact_NoSd(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Fast_NoSd
{
public:
  Mx_DisplayH_ILI9341_Fast_NoSd(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Fastest_NoSd
{
public:
  Mx_DisplayH_ILI9341_Fastest_NoSd(bool rotate180);
  MxMethodDefinitions;
};


// --- Config: NoSd_NoSerial ---

class Mx_DisplayV_ILI9341_CompactMinimal_NoSd_NoSerial
{
public:
  Mx_DisplayV_ILI9341_CompactMinimal_NoSd_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Compact_NoSd_NoSerial
{
public:
  Mx_DisplayV_ILI9341_Compact_NoSd_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Fast_NoSd_NoSerial
{
public:
  Mx_DisplayV_ILI9341_Fast_NoSd_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial
{
public:
  Mx_DisplayV_ILI9341_Fastest_NoSd_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_CompactMinimal_NoSd_NoSerial
{
public:
  Mx_DisplayH_ILI9341_CompactMinimal_NoSd_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Compact_NoSd_NoSerial
{
public:
  Mx_DisplayH_ILI9341_Compact_NoSd_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Fast_NoSd_NoSerial
{
public:
  Mx_DisplayH_ILI9341_Fast_NoSd_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

class Mx_DisplayH_ILI9341_Fastest_NoSd_NoSerial
{
public:
  Mx_DisplayH_ILI9341_Fastest_NoSd_NoSerial(bool rotate180);
  MxMethodDefinitions;
};

#undef MxMethodDefinitions

#endif
