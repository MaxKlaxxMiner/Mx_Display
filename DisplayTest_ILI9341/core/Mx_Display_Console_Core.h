template<typename Mx_Display>
class MxClassName
{
public:
  uint16_t foregroundColor;
  uint16_t backgroundColor;
  int16_t currentX;
  int16_t currentY;
  uint8_t charSizeW;
  uint8_t charSizeH;
  int16_t paddingLeft;
  int16_t paddingRight;
  int16_t paddingTop;
  int16_t paddingBottom;
  bool autoWrap;
  bool autoScroll;

  MxClassName(Mx_Display_Scrollable<Mx_Display>* displayScrollable)
  {
    scr = displayScrollable;
    foregroundColor = color565(0, 128, 255);
    backgroundColor = 0x0000;
    charSizeW = charSizeH = 1;
    paddingLeft = paddingRight = paddingTop = paddingBottom = 0;
    autoWrap = true;
    autoScroll = true;
    currentX = paddingLeft;
    currentY = paddingTop;
  }

#ifndef MxCompact
  void drawChar(uint16_t x, uint16_t y, uint8_t c)
  {
    uint16_t p = scr->setWindowLine(x, y, x + 4);
    uint8_t* fontP = &fontAdafruit[c];
    uint8_t line0 = pgm_read_byte(fontP);
    uint8_t line1 = pgm_read_byte(fontP + 0x100);
    uint8_t line2 = pgm_read_byte(fontP + 0x200);
    uint8_t line3 = pgm_read_byte(fontP + 0x300);
    uint8_t line4 = pgm_read_byte(fontP + 0x400);
    uint16_t fColor = foregroundColor;
    uint16_t bColor = backgroundColor;

#ifdef MxFast
    if (p <= scr->height() - 8)
    {
      scr->fillWindowPixel(line0 & 1 ? fColor : bColor);
      scr->fillWindowPixel(line1 & 1 ? fColor : bColor);
      scr->fillWindowPixel(line2 & 1 ? fColor : bColor);
      scr->fillWindowPixel(line3 & 1 ? fColor : bColor);
      scr->fillWindowPixel(line4 & 1 ? fColor : bColor);
      scr->fillWindowPixel(line0 & 2 ? fColor : bColor);
      scr->fillWindowPixel(line1 & 2 ? fColor : bColor);
      scr->fillWindowPixel(line2 & 2 ? fColor : bColor);
      scr->fillWindowPixel(line3 & 2 ? fColor : bColor);
      scr->fillWindowPixel(line4 & 2 ? fColor : bColor);
      scr->fillWindowPixel(line0 & 4 ? fColor : bColor);
      scr->fillWindowPixel(line1 & 4 ? fColor : bColor);
      scr->fillWindowPixel(line2 & 4 ? fColor : bColor);
      scr->fillWindowPixel(line3 & 4 ? fColor : bColor);
      scr->fillWindowPixel(line4 & 4 ? fColor : bColor);
      scr->fillWindowPixel(line0 & 8 ? fColor : bColor);
      scr->fillWindowPixel(line1 & 8 ? fColor : bColor);
      scr->fillWindowPixel(line2 & 8 ? fColor : bColor);
      scr->fillWindowPixel(line3 & 8 ? fColor : bColor);
      scr->fillWindowPixel(line4 & 8 ? fColor : bColor);
      scr->fillWindowPixel(line0 & 16 ? fColor : bColor);
      scr->fillWindowPixel(line1 & 16 ? fColor : bColor);
      scr->fillWindowPixel(line2 & 16 ? fColor : bColor);
      scr->fillWindowPixel(line3 & 16 ? fColor : bColor);
      scr->fillWindowPixel(line4 & 16 ? fColor : bColor);
      scr->fillWindowPixel(line0 & 32 ? fColor : bColor);
      scr->fillWindowPixel(line1 & 32 ? fColor : bColor);
      scr->fillWindowPixel(line2 & 32 ? fColor : bColor);
      scr->fillWindowPixel(line3 & 32 ? fColor : bColor);
      scr->fillWindowPixel(line4 & 32 ? fColor : bColor);
      scr->fillWindowPixel(line0 & 64 ? fColor : bColor);
      scr->fillWindowPixel(line1 & 64 ? fColor : bColor);
      scr->fillWindowPixel(line2 & 64 ? fColor : bColor);
      scr->fillWindowPixel(line3 & 64 ? fColor : bColor);
      scr->fillWindowPixel(line4 & 64 ? fColor : bColor);
      scr->fillWindowPixel(line0 & 128 ? fColor : bColor);
      scr->fillWindowPixel(line1 & 128 ? fColor : bColor);
      scr->fillWindowPixel(line2 & 128 ? fColor : bColor);
      scr->fillWindowPixel(line3 & 128 ? fColor : bColor);
      scr->fillWindowPixel(line4 & 128 ? fColor : bColor);
    }
    else
#endif
    {
      for (uint8_t j = 0; j < 8; j++)
      {
        scr->fillWindowPixel(line0 & 1 ? fColor : bColor); line0 >>= 1;
        scr->fillWindowPixel(line1 & 1 ? fColor : bColor); line1 >>= 1;
        scr->fillWindowPixel(line2 & 1 ? fColor : bColor); line2 >>= 1;
        scr->fillWindowPixel(line3 & 1 ? fColor : bColor); line3 >>= 1;
        scr->fillWindowPixel(line4 & 1 ? fColor : bColor); line4 >>= 1;
        p++; y++; if (p == scr->height()) p = scr->setWindowLine(x, y, x + 4);
      }
    }
  }

  void drawCharZoom(uint16_t x, uint16_t y, uint8_t c, uint8_t w, uint8_t h)
  {
    uint16_t p = scr->setWindowLine(x, y, x + (uint8_t)(w * (uint8_t)5) - 1);
    uint8_t* fontP = &fontAdafruit[c];
    uint8_t line0 = pgm_read_byte(fontP);
    uint8_t line1 = pgm_read_byte(fontP + 0x100);
    uint8_t line2 = pgm_read_byte(fontP + 0x200);
    uint8_t line3 = pgm_read_byte(fontP + 0x300);
    uint8_t line4 = pgm_read_byte(fontP + 0x400);
    uint16_t fColor = foregroundColor;
    uint16_t bColor = backgroundColor;

    for (uint8_t j = 0; j < 8; j++)
    {
      for (uint8_t r = 0; r < h; r++)
      {
        scr->fillWindowPixel(line0 & 1 ? fColor : bColor, w);
        scr->fillWindowPixel(line1 & 1 ? fColor : bColor, w);
        scr->fillWindowPixel(line2 & 1 ? fColor : bColor, w);
        scr->fillWindowPixel(line3 & 1 ? fColor : bColor, w);
        scr->fillWindowPixel(line4 & 1 ? fColor : bColor, w);
        p++; y++; if (p == scr->height()) p = scr->setWindowLine(x, y, x + (uint8_t)(w * (uint8_t)5) - 1);
      }
      line0 >>= 1; line1 >>= 1; line2 >>= 1; line3 >>= 1; line4 >>= 1;
    }
  }
#endif

  void drawCharSafe(int16_t x, int16_t y, uint8_t c, uint8_t w, uint8_t h)
  {
    if (x >= scr->width() || y >= scr->height() || x + w * 6 < 0 || y + h * 8 < 0) return;

    uint8_t* fontP = &fontAdafruit[c];
#ifdef MxMinimal
    uint8_t line[5];
    for (uint8_t n = 0; n < 5; n++, fontP += 0x100) line[n] = pgm_read_byte(fontP);
    uint16_t fColor = foregroundColor;
    uint16_t bColor = backgroundColor;

    for (uint8_t j = 0; j < 8; j++)
    {
      for (uint8_t r = 0; r < h; r++, y++)
      {
        if ((uint16_t)y >= scr->height()) continue;
        int16_t cx = x;
        for (int n = 0; n < 5; n++)
        {
          for (uint8_t i = 0; i < w; i++, cx++)
          {
            if ((uint16_t)cx < scr->width() && (fColor != bColor || (line[n] & 1)))
            {
              scr->drawPixel(cx, y, line[n] & 1 ? fColor : bColor);
            }
          }
        }
      }
      line[0] >>= 1; line[1] >>= 1; line[2] >>= 1; line[3] >>= 1; line[4] >>= 1;
    }
#else
    uint8_t line0 = pgm_read_byte(fontP);
    uint8_t line1 = pgm_read_byte(fontP + 0x100);
    uint8_t line2 = pgm_read_byte(fontP + 0x200);
    uint8_t line3 = pgm_read_byte(fontP + 0x300);
    uint8_t line4 = pgm_read_byte(fontP + 0x400);
    uint16_t fColor = foregroundColor;
    uint16_t bColor = backgroundColor;

    for (uint8_t j = 0; j < 8; j++)
    {
      for (uint8_t r = 0; r < h; r++, y++)
      {
        if ((uint16_t)y >= scr->height()) continue;
        int16_t cx = x;
        if (fColor == bColor)
        {
          for (uint8_t i = 0; i < w; i++, cx++) if ((uint16_t)cx < scr->width() && (line0 & 1)) scr->drawPixel(cx, y, fColor);
          for (uint8_t i = 0; i < w; i++, cx++) if ((uint16_t)cx < scr->width() && (line1 & 1)) scr->drawPixel(cx, y, fColor);
          for (uint8_t i = 0; i < w; i++, cx++) if ((uint16_t)cx < scr->width() && (line2 & 1)) scr->drawPixel(cx, y, fColor);
          for (uint8_t i = 0; i < w; i++, cx++) if ((uint16_t)cx < scr->width() && (line3 & 1)) scr->drawPixel(cx, y, fColor);
          for (uint8_t i = 0; i < w; i++, cx++) if ((uint16_t)cx < scr->width() && (line4 & 1)) scr->drawPixel(cx, y, fColor);
        }
        else
        {
          for (uint8_t i = 0; i < w; i++, cx++) if ((uint16_t)cx < scr->width()) scr->drawPixel(cx, y, line0 & 1 ? fColor : bColor);
          for (uint8_t i = 0; i < w; i++, cx++) if ((uint16_t)cx < scr->width()) scr->drawPixel(cx, y, line1 & 1 ? fColor : bColor);
          for (uint8_t i = 0; i < w; i++, cx++) if ((uint16_t)cx < scr->width()) scr->drawPixel(cx, y, line2 & 1 ? fColor : bColor);
          for (uint8_t i = 0; i < w; i++, cx++) if ((uint16_t)cx < scr->width()) scr->drawPixel(cx, y, line3 & 1 ? fColor : bColor);
          for (uint8_t i = 0; i < w; i++, cx++) if ((uint16_t)cx < scr->width()) scr->drawPixel(cx, y, line4 & 1 ? fColor : bColor);
        }
      }
      line0 >>= 1; line1 >>= 1; line2 >>= 1; line3 >>= 1; line4 >>= 1;
    }
#endif
  }

  void newLine()
  {
    currentX = paddingLeft;
    currentY += charSizeH * 8;
    if (autoScroll)
    {
      int16_t reserve = scr->height() - currentY - paddingBottom - charSizeH * 8;
      if (reserve < 0)
      {
#ifdef MxMinimal
        for (int16_t y = 0; y < -reserve; y++)
        {
          for (int16_t x = 0; x < scr->width(); x++)
          {
            scr->drawPixel(x, y, backgroundColor);
          }
        }
#else
        scr->fillRect(0, 0, scr->width(), -reserve, backgroundColor);
#endif
        scr->scroll(-reserve);
        currentY += reserve;
      }
    }
    else
    {
      if (currentY > scr->height()) currentY = scr->height();
    }
  }

  size_t write(uint8_t c)
  {
    if (c < ' ')
    {
      if (c == '\n') { newLine(); return 1; }
      if (c == '\r') return 1;
    }
#ifndef MxCompact
    if ((uint16_t)(currentX + charSizeW * 6) < scr->width() && (uint16_t)(currentY + charSizeH * 8) < scr->height())
    {
      if (charSizeW + charSizeH == 2) drawChar(currentX, currentY, c); else drawCharZoom(currentX, currentY, c, charSizeW, charSizeH);
    }
    else
#endif
    {
      drawCharSafe(currentX, currentY, c, charSizeW, charSizeH);
    }
    currentX += charSizeW * 6;
    if (autoWrap)
    {
      if (currentX + charSizeW * 6 + paddingRight > scr->width()) newLine();
    }
    else
    {
      if (currentX > scr->width()) currentX = scr->width();
    }
    return 1;
  }

  // --- Print-Clone (tuned) ---

  size_t write(const uint8_t* buffer, size_t size)
  {
    for (int i = 0; i < size; i++)
    {
      write(buffer[i]);
    }
    return size;
  }

  size_t write(const char* str)
  {
    if (str == NULL) return 0;
    return write((const uint8_t*)str, strlen(str));
  }

  size_t write(const char* buffer, size_t size)
  {
    return write((const uint8_t*)buffer, size);
  }

  size_t print(const __FlashStringHelper* ifsh)
  {
    PGM_P p = reinterpret_cast<PGM_P>(ifsh);
    size_t n = 0;
    while (1)
    {
      unsigned char c = pgm_read_byte(p++);
      if (c == 0) break;
      if (write(c)) n++;
      else break;
    }
    return n;
  }

  size_t print(const String& s)
  {
    return write(s.c_str(), s.length());
  }

  size_t print(const char str[])
  {
    return write(str);
  }

  size_t print(char c)
  {
    return write(c);
  }

  size_t print(unsigned char b, int base)
  {
    return print((unsigned long)b, base);
  }

  size_t print(int n, int base)
  {
    return print((long)n, base);
  }

  size_t print(unsigned int n, int base)
  {
    return print((unsigned long)n, base);
  }

  size_t print(long n, int base)
  {
    if (base == 0)
    {
      return write(n);
    }
    else if (base == 10)
    {
      if (n < 0)
      {
        int t = print('-');
        n = -n;
        return printNumber(n) + t;
      }
      return printNumber(n);
    }
    else
    {
      return printNumber(n, base);
    }
  }

  size_t print(unsigned long n, int base)
  {
    if (base == 0) return write(n);
    else return printNumber(n, base);
  }

  size_t print(double n, int digits = 2)
  {
    return printFloat(n, digits);
  }

  size_t println(const __FlashStringHelper* ifsh)
  {
    size_t n = print(ifsh);
    n += println();
    return n;
  }

  size_t print(const Printable& x)
  {
    return x.printTo(*this);
  }

  size_t println(void)
  {
    write('\n');
    return 2;
  }

  size_t println(const String& s)
  {
    size_t n = print(s);
    n += println();
    return n;
  }

  size_t println(const char c[])
  {
    size_t n = print(c);
    n += println();
    return n;
  }

  size_t println(char c)
  {
    size_t n = print(c);
    n += println();
    return n;
  }

  size_t println(unsigned char b, int base)
  {
    size_t n = print(b, base);
    n += println();
    return n;
  }

  size_t println(int num, int base)
  {
    size_t n = print(num, base);
    n += println();
    return n;
  }

  size_t println(unsigned int num, int base)
  {
    size_t n = print(num, base);
    n += println();
    return n;
  }

  size_t println(long num, int base)
  {
    size_t n = print(num, base);
    n += println();
    return n;
  }

  size_t println(unsigned long num, int base)
  {
    size_t n = print(num, base);
    n += println();
    return n;
  }

  size_t println(double num, int digits = 2)
  {
    size_t n = print(num, digits);
    n += println();
    return n;
  }

  size_t println(const Printable& x)
  {
    size_t n = print(x);
    n += println();
    return n;
  }

  size_t print(unsigned char b)
  {
    return print((unsigned long)b);
  }

  size_t print(int n)
  {
    return print((long)n);
  }

  size_t print(unsigned int n)
  {
    return print((unsigned long)n);
  }

  size_t print(long n)
  {
    if (n < 0)
    {
      int t = print('-');
      n = -n;
      return printNumber(n) + t;
    }
    return printNumber(n);
  }

  size_t print(unsigned long n)
  {
    return printNumber(n);
  }

  size_t println(unsigned char b)
  {
    size_t n = print(b);
    n += println();
    return n;
  }

  size_t println(int num)
  {
    size_t n = print(num);
    n += println();
    return n;
  }

  size_t println(unsigned int num)
  {
    size_t n = print(num);
    n += println();
    return n;
  }

  size_t println(long num)
  {
    size_t n = print(num);
    n += println();
    return n;
  }

  size_t println(unsigned long num)
  {
    size_t n = print(num);
    n += println();
    return n;
  }

private:
  Mx_Display_Scrollable<Mx_Display>* scr;

  size_t printFloat(double number, uint8_t digits = 2)
  {
    size_t n = 0;

    if (isnan(number)) return print("nan");
    if (isinf(number)) return print("inf");
    if (number > 4294967040.0) return print("ovf");  // constant determined empirically
    if (number < -4294967040.0) return print("ovf");  // constant determined empirically

    // Handle negative numbers
    if (number < 0.0)
    {
      n += print('-');
      number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
      rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    n += print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
    {
      n += print('.');
    }

    // Extract digits from the remainder one at a time
    while (digits-- > 0)
    {
      remainder *= 10.0;
      unsigned int toPrint = (unsigned int)(remainder);
      n += print(toPrint);
      remainder -= toPrint;
    }

    return n;
  }

  size_t printNumber(unsigned long n, uint8_t base)
  {
    char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    char* str = &buf[sizeof(buf) - 1];

    *str = '\0';

    // prevent crash if called with base == 1
    if (base < 2) base = 10;

    do
    {
      char c = n % base;
      n /= base;

      *--str = c < 10 ? c + '0' : c + 'A' - 10;
    }
    while (n);

    return write(str);
  }

  size_t printNumber(unsigned long n)
  {
    char buf[10]; // Assumes 8-bit chars plus zero byte.
    char* str = &buf[10];

    do
    {
      char c = n % 10;
      n /= 10;

      *--str = c + '0';
    }
    while (n);

    return write(str, (size_t)(&buf[10] - str));
  }
};
