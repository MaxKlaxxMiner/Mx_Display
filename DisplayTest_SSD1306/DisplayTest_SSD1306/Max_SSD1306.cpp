#include <avr/pgmspace.h>
#include "glcdfont.h"
#include "Max_SSD1306.h"

Max_SSD1306::Max_SSD1306(TwoMaxWire* twi) : wire(twi ? twi : &MaxWire), buffer(NULL) {}

Max_SSD1306::~Max_SSD1306(void)
{
  if (buffer)
  {
    free(buffer);
    buffer = NULL;
  }
}

void Max_SSD1306::ssd1306_command(byte c)
{
  wire->beginTransmission(i2caddr);
  wire->write(0x00); // Co = 0, D/C = 0
  wire->write(c);
  wire->endTransmission();
}

void Max_SSD1306::ssd1306_command(byte c, byte v)
{
  wire->beginTransmission(i2caddr);
  wire->write(0x00); // Co = 0, D/C = 0
  wire->write(c);
  wire->write(v);
  wire->endTransmission();
}

void Max_SSD1306::ssd1306_commandList(const uint8_t* c, uint8_t n)
{
  wire->beginTransmission(i2caddr);
  wire->write(0x00); // Co = 0, D/C = 0
  while (n--) wire->write(pgm_read_byte(c++));
  wire->endTransmission();
}

bool Max_SSD1306::begin(byte addr, uint32_t wireClock)
{
  if ((!buffer) && !(buffer = (byte*)malloc(BUFFER_BYTES + 1) + 1)) return false;

  clearDisplay();

  i2caddr = addr;
  wire->begin();
  wire->setClock(wireClock);

  static const byte PROGMEM init[] =
  {
    SSD1306_DISPLAYOFF,                // 0xAE
    SSD1306_SETDISPLAYCLOCKDIV, 0x80,  // 0xD5
    SSD1306_SETMULTIPLEX, HEIGHT - 1,  // 0xA8

    SSD1306_SETDISPLAYOFFSET, 0x00,    // 0xD3
    SSD1306_SETSTARTLINE | 0x0,        // line #0

    SSD1306_MEMORYMODE, 0x00,          // 0x20
    SSD1306_SEGREMAP | 0x1,
    SSD1306_COMSCANDEC,
    SSD1306_SETCOMPINS, 0x12,
    SSD1306_SETPRECHARGE, 0xf1,

    SSD1306_SETVCOMDETECT, 0x40,       // 0xDB
    SSD1306_DISPLAYALLON_RESUME,       // 0xA4
    SSD1306_NORMALDISPLAY,             // 0xA6
    SSD1306_DEACTIVATE_SCROLL,
    SSD1306_DISPLAYON,                 // Main screen turn on
    SSD1306_CHARGEPUMP, 0x14,
    SSD1306_SETCONTRAST, 0x9f,
  };
  ssd1306_commandList(init, sizeof(init));

  textcolor = 1;
  textsize_x = 1;
  textsize_y = 1;
  textsize_w = 6;
  wrap = true;

  return true; // Success
}

void Max_SSD1306::clearDisplay(byte color = 0)
{
  if (color) color = 0xff;
  memset(buffer, color, BUFFER_BYTES);
}

void Max_SSD1306::invertDisplay(bool invert)
{
  ssd1306_command(invert ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}

void Max_SSD1306::dim(bool dim)
{
  ssd1306_command(SSD1306_SETCONTRAST, dim ? 0x00 : 0x9f);
}

void Max_SSD1306::pump(bool chargePump)
{
  ssd1306_command(SSD1306_CHARGEPUMP, chargePump ? 0x14 : 0x10);
}

void Max_SSD1306::drawPixel(int x, int y, byte color)
{
  if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT))
  {
    buffer[x + (y / 8) * WIDTH] ^= (color << (y & 7));
  }
}

void Max_SSD1306::display(void)
{
  static const uint8_t PROGMEM dlist[] =
  {
    SSD1306_PAGEADDR,
    0,                         // Page start address
    0xff,                      // Page end (not really, but works here)
    SSD1306_COLUMNADDR,
    0,
    WIDTH - 1
  };                       // Column start address
  ssd1306_commandList(dlist, sizeof(dlist));

  int count = WIDTH * ((HEIGHT + 7) / 8);
  wire->directSend(i2caddr, 0x40, buffer, count);
}

void Max_SSD1306::setTextSize(byte size)
{
  setTextSize(size, size);
}

void Max_SSD1306::setTextSize(byte sizeX, byte sizeY)
{
  textsize_x = (sizeX > 0) ? sizeX : 1;
  textsize_y = (sizeY > 0) ? sizeY : 1;
  textsize_w = sizeX * 6;
}

void Max_SSD1306::setTextColor(byte color)
{
  textcolor = color;
}

void Max_SSD1306::setCursor(byte x, byte y)
{
  cursor_x = x;
  cursor_y = y;
}

size_t Max_SSD1306::write(byte c)
{
  if (c == '\n')
  {              // Newline?
    cursor_x = 0;               // Reset x to zero,
    cursor_y += textsize_y * 8; // advance y one line
  }
  else if (c != '\r')
  {       // Ignore carriage returns
    if (wrap && ((cursor_x + textsize_w) > WIDTH))
    { // Off right?
      cursor_x = 0;                                       // Reset x to zero,
      cursor_y += textsize_y * 8; // advance y one line
    }
    if (textsize_x + textsize_y == 2) drawChar(cursor_x, cursor_y, c, textcolor); else drawChar(cursor_x, cursor_y, c, textcolor, textsize_x, textsize_y);
    cursor_x += textsize_w; // Advance x one char
  }
  return true;
}

void Max_SSD1306::drawChar(byte x, byte y, byte c, byte color)
{
  if (x > WIDTH - 6 || y > HEIGHT - 8) return;

  byte* bufP = &buffer[y / 8 * WIDTH + x];
  byte* charP = &font[c * 5];

  *(uint16_t*)(bufP + 0) = pgm_read_word(charP + 0);
  *(uint16_t*)(bufP + 2) = pgm_read_word(charP + 2);
  *(bufP + 4) = pgm_read_byte(charP + 4);
}

void Max_SSD1306::drawChar(byte x, byte y, byte c, byte color, byte size_x, byte size_y)
{
  //if ((x >= WIDTH) || (y >= HEIGHT) || ((x + 6 * size_x - 1) < 0) || ((y + 8 * size_y - 1) < 0)) return;

  //for (int8_t i = 0; i < 5; i++)
  //{ // Char bitmap = 5 columns
  //  uint8_t line = pgm_read_byte(&font[c * 5 + i]);
  //  for (int8_t j = 0; j < 8; j++, line >>= 1)
  //  {
  //    if (line & 1)
  //    {
  //      writeFillRect(x + i * size_x, y + j * size_y, size_x, size_y, color);
  //    }
  //  }
  //}
}
