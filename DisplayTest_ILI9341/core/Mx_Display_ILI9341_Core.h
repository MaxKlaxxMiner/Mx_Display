#ifdef MxPinFixed
#define rdIdle() digitalWriteDirect(TP28017_PIN_RD, HIGH);
#define rdActive() digitalWriteDirect(TP28017_PIN_RD, LOW);
#define wrIdle() digitalWriteDirect(TP28017_PIN_WR, HIGH);
#define wrActive() digitalWriteDirect(TP28017_PIN_WR, LOW);
#define cdData() digitalWriteDirect(TP28017_PIN_CD, HIGH);
#define cdCommand() digitalWriteDirect(TP28017_PIN_CD, LOW);
#define csIdle() digitalWriteDirect(TP28017_PIN_CS, HIGH);
#define csActive() digitalWriteDirect(TP28017_PIN_CS, LOW);
#else // Reference
void MxClassName::rdIdle() { digitalWrite(_rdPin, HIGH); }
void MxClassName::rdActive() { digitalWrite(_rdPin, LOW); }
void MxClassName::wrIdle() { digitalWrite(_wrPin, HIGH); }
void MxClassName::wrActive() { digitalWrite(_wrPin, LOW); }
void MxClassName::cdData() { digitalWrite(_cdPin, HIGH); }
void MxClassName::cdCommand() { digitalWrite(_cdPin, LOW); }
void MxClassName::csIdle() { digitalWrite(_csPin, HIGH); }
void MxClassName::csActive() { digitalWrite(_csPin, LOW); }
#endif

#if defined(__AVR_ATmega328P__)
#define wrStrobeCmd() { PORTC = B10001; PORTC = B10011; }
#define wrStrobeData() { PORTC = B10101; PORTC = B10111; }
#elif defined(__AVR_ATmega2560__)
#define wrStrobeCmd() { PORTF = B10001; PORTF = B10011; }
#define wrStrobeData() { PORTF = B10101; PORTF = B10111; }
#else
#define wrStrobeCmd() { wrActive(); wrIdle(); }
#define wrStrobeData() { wrActive(); wrIdle(); }
#endif

#ifdef MxPinFixed
#if defined(__AVR_ATmega328P__)
#ifdef MxMod
#define write8inline(val) \
{                         \
  PORTD = (val);          \
}
#elif defined(MxNoSerial) && defined(MxNoSd)
#define write8inline(val)                            \
{                                                    \
  PORTD = (val);                                     \
  PORTB = (val);                                     \
}
#elif defined(MxNoSerial) && !defined(MxNoSd)
#define write8inline(val)                            \
{                                                    \
  PORTD = (val);                                     \
  PORTB = (PORTB & B11111100) | ((val) & B00000011); \
}
#elif !defined(MxNoSerial) && defined(MxNoSd)
#define write8inline(val)                            \
{                                                    \
  PORTD = (PORTD & B00000011) | ((val) & B11111100); \
  PORTB = (val);                                     \
}
#else
#define write8inline(val)                            \
{                                                    \
  PORTD = (PORTD & B00000011) | ((val) & B11111100); \
  PORTB = (PORTB & B11111100) | ((val) & B00000011); \
}
#endif
#elif defined(__AVR_ATmega2560__) && !defined(MxMinimal)
#ifdef MxMod
#define write8inline(val)                                                            \
{                                                                                    \
  PORTE = ((val) & B00000011) | ((val) & B00001100) << 2 | ((val) & B00100000) >> 2; \
  PORTG = (PORTG & B11011111) | ((val) & B00010000) << 1;                            \
  PORTH = (PORTH & B11100111) | ((val) & B11000000) >> 3;                            \
}
#else
#define write8inline(val)                                                            \
{                                                                                    \
  PORTE = ((val) & B00001100) << 2 | ((val) & B00100000) >> 2;                       \
  PORTG = (PORTG & B11011111) | ((val) & B00010000) << 1;                            \
  PORTH = (PORTH & B10000111) | ((val) & B00000011) << 5 | ((val) & B11000000) >> 3; \
}
#endif
#else
#ifdef MxMod
#define write8inline(val)                                              \
{                                                                      \
  digitalWriteDirect(TP28017_PIN_DATA0_MOD, ((val) & B00000001) >> 0); \
  digitalWriteDirect(TP28017_PIN_DATA1_MOD, ((val) & B00000010) >> 1); \
  digitalWriteDirect(TP28017_PIN_DATA2, ((val) & B00000100) >> 2);     \
  digitalWriteDirect(TP28017_PIN_DATA3, ((val) & B00001000) >> 3);     \
  digitalWriteDirect(TP28017_PIN_DATA4, ((val) & B00010000) >> 4);     \
  digitalWriteDirect(TP28017_PIN_DATA5, ((val) & B00100000) >> 5);     \
  digitalWriteDirect(TP28017_PIN_DATA6, ((val) & B01000000) >> 6);     \
  digitalWriteDirect(TP28017_PIN_DATA7, ((val) & B10000000) >> 7);     \
}
#else
#define write8inline(val)                                          \
{                                                                  \
  digitalWriteDirect(TP28017_PIN_DATA0, ((val) & B00000001) >> 0); \
  digitalWriteDirect(TP28017_PIN_DATA1, ((val) & B00000010) >> 1); \
  digitalWriteDirect(TP28017_PIN_DATA2, ((val) & B00000100) >> 2); \
  digitalWriteDirect(TP28017_PIN_DATA3, ((val) & B00001000) >> 3); \
  digitalWriteDirect(TP28017_PIN_DATA4, ((val) & B00010000) >> 4); \
  digitalWriteDirect(TP28017_PIN_DATA5, ((val) & B00100000) >> 5); \
  digitalWriteDirect(TP28017_PIN_DATA6, ((val) & B01000000) >> 6); \
  digitalWriteDirect(TP28017_PIN_DATA7, ((val) & B10000000) >> 7); \
}
#endif
#endif
#else // Reference
#define write8inline(val)                          \
{                                                  \
  digitalWrite(_data0Pin, (val & B00000001) >> 0); \
  digitalWrite(_data1Pin, (val & B00000010) >> 1); \
  digitalWrite(_data2Pin, (val & B00000100) >> 2); \
  digitalWrite(_data3Pin, (val & B00001000) >> 3); \
  digitalWrite(_data4Pin, (val & B00010000) >> 4); \
  digitalWrite(_data5Pin, (val & B00100000) >> 5); \
  digitalWrite(_data6Pin, (val & B01000000) >> 6); \
  digitalWrite(_data7Pin, (val & B10000000) >> 7); \
}
#endif

void MxClassName::write8(uint8_t val)
{
  write8inline(val);
}

uint8_t MxClassName::read8()
{
  asm volatile("rjmp .+0\n" ::);
  uint8_t val;
#if defined(MxReference)
  val = digitalRead(_data0Pin) ? B00000001 : 0;
  val |= digitalRead(_data1Pin) ? B00000010 : 0;
  val |= digitalRead(_data2Pin) ? B00000100 : 0;
  val |= digitalRead(_data3Pin) ? B00001000 : 0;
  val |= digitalRead(_data4Pin) ? B00010000 : 0;
  val |= digitalRead(_data5Pin) ? B00100000 : 0;
  val |= digitalRead(_data6Pin) ? B01000000 : 0;
  val |= digitalRead(_data7Pin) ? B10000000 : 0;
#elif defined(__AVR_ATmega328P__)
#ifdef MxMod
  val = PIND;
#else
  val = (PIND & B11111100) | (PINB & B00000011);
#endif
#elif defined(__AVR_ATmega2560__)
#ifdef MxMod
  val = (PINE & B00000011) | (PINE & B00110000) >> 2 | (PINE & B00001000) << 2 | (PING & B00100000) >> 1 | (PINH & B00011000) << 3;
#else
  val = (PINE & B00110000) >> 2 | (PINE & B00001000) << 2 | (PING & B00100000) >> 1 | (PINH & B01100000) >> 5 | (PINH & B00011000) << 3;
#endif
#else
#ifdef MxMod
  val = digitalRead(TP28017_PIN_DATA0_MOD) ? B00000001 : 0;
  val |= digitalRead(TP28017_PIN_DATA1_MOD) ? B00000010 : 0;
#else
  val = digitalRead(TP28017_PIN_DATA0) ? B00000001 : 0;
  val |= digitalRead(TP28017_PIN_DATA1) ? B00000010 : 0;
#endif
  val |= digitalRead(TP28017_PIN_DATA2) ? B00000100 : 0;
  val |= digitalRead(TP28017_PIN_DATA3) ? B00001000 : 0;
  val |= digitalRead(TP28017_PIN_DATA4) ? B00010000 : 0;
  val |= digitalRead(TP28017_PIN_DATA5) ? B00100000 : 0;
  val |= digitalRead(TP28017_PIN_DATA6) ? B01000000 : 0;
  val |= digitalRead(TP28017_PIN_DATA7) ? B10000000 : 0;
#endif
  return val;
}

#define commandInline(cmd) { write8inline(cmd); wrStrobeCmd(); }

void MxClassName::command(uint8_t cmd)
{
  write8(cmd); wrStrobeCmd();
}

void MxClassName::command(uint8_t cmd, uint8_t val)
{
  command(cmd);
  write8(val); wrStrobeData();
}

void MxClassName::command(uint8_t cmd, uint8_t val1, uint8_t val2)
{
#ifdef MxMinimal
  command(cmd, val1);
  write8(val2); wrStrobeData();
#else
  command(cmd);
  write8(val1); wrStrobeData();
  write8(val2); wrStrobeData();
#endif
}

void MxClassName::command16(uint8_t cmd, uint16_t val1, uint16_t val2)
{
#ifdef MxMinimal
  command(cmd, val1 >> 8, val1);
  write8(val2 >> 8); wrStrobeData(); write8(val2); wrStrobeData();
#else
  command(cmd);
  write8(val1 >> 8); wrStrobeData(); write8(val1); wrStrobeData();
  write8(val2 >> 8); wrStrobeData(); write8(val2); wrStrobeData();
#endif
}

void MxClassName::setWriteDir()
{
#ifdef MxPinFixed
#if defined(__AVR_ATmega328P__)
#ifdef MxMod
  DDRD = B11111111;
#else
  DDRB |= B00000011;
  DDRD |= B11111100;
#endif
#elif defined(__AVR_ATmega2560__)
#ifdef MxMod
  DDRE |= B00111011;
  DDRG |= B00100000;
  DDRH |= B00011000;
#else
  DDRE |= B00111000;
  DDRG |= B00100000;
  DDRH |= B01111000;
#endif
#else
#ifdef MxMod
  pinModeDirect(TP28017_PIN_DATA0_MOD, OUTPUT);
  pinModeDirect(TP28017_PIN_DATA1_MOD, OUTPUT);
#else
  pinModeDirect(TP28017_PIN_DATA0, OUTPUT);
  pinModeDirect(TP28017_PIN_DATA1, OUTPUT);
#endif
  pinModeDirect(TP28017_PIN_DATA2, OUTPUT);
  pinModeDirect(TP28017_PIN_DATA3, OUTPUT);
  pinModeDirect(TP28017_PIN_DATA4, OUTPUT);
  pinModeDirect(TP28017_PIN_DATA5, OUTPUT);
  pinModeDirect(TP28017_PIN_DATA6, OUTPUT);
  pinModeDirect(TP28017_PIN_DATA7, OUTPUT);
#endif
#else
  pinMode(_data0Pin, OUTPUT);
  pinMode(_data1Pin, OUTPUT);
  pinMode(_data2Pin, OUTPUT);
  pinMode(_data3Pin, OUTPUT);
  pinMode(_data4Pin, OUTPUT);
  pinMode(_data5Pin, OUTPUT);
  pinMode(_data6Pin, OUTPUT);
  pinMode(_data7Pin, OUTPUT);
#endif
}

void MxClassName::setReadDir()
{
#ifdef MxPinFixed
#if defined(__AVR_ATmega328P__)
#ifdef MxMod
  DDRD = B00000000;
#else
  DDRB &= ~B00000011;
  DDRD &= ~B11111100;
#endif
#elif defined(__AVR_ATmega2560__)
#ifdef MxMod
  DDRE &= ~B00111011;
  DDRG &= ~B00100000;
  DDRH &= ~B00011000;
#else
  DDRE &= ~B00111000;
  DDRG &= ~B00100000;
  DDRH &= ~B01111000;
#endif
#else
#ifdef MxMod
  pinModeDirect(TP28017_PIN_DATA0_MOD, INPUT);
  pinModeDirect(TP28017_PIN_DATA1_MOD, INPUT);
#else
  pinModeDirect(TP28017_PIN_DATA0, INPUT);
  pinModeDirect(TP28017_PIN_DATA1, INPUT);
#endif
  pinModeDirect(TP28017_PIN_DATA2, INPUT);
  pinModeDirect(TP28017_PIN_DATA3, INPUT);
  pinModeDirect(TP28017_PIN_DATA4, INPUT);
  pinModeDirect(TP28017_PIN_DATA5, INPUT);
  pinModeDirect(TP28017_PIN_DATA6, INPUT);
  pinModeDirect(TP28017_PIN_DATA7, INPUT);
#endif
#else
  pinMode(_data0Pin, INPUT);
  pinMode(_data1Pin, INPUT);
  pinMode(_data2Pin, INPUT);
  pinMode(_data3Pin, INPUT);
  pinMode(_data4Pin, INPUT);
  pinMode(_data5Pin, INPUT);
  pinMode(_data6Pin, INPUT);
  pinMode(_data7Pin, INPUT);
#endif
}

bool MxClassName::begin(bool rotate180)
{
#ifdef MxPinFixed
#if defined(__AVR_ATmega328P__)
  PORTC |= B00010111; // rdIdle(); + wrIdle(); + csIdle(); + digitalWriteDirect(TP28017_PIN_RS, HIGH);
  DDRC |= B00011111; // pinMode OUTPUT: TP28017_PIN_RD + TP28017_PIN_WR + TP28017_PIN_CD + TP28017_PIN_CS + TP28017_PIN_RS
#else
  rdIdle();
  wrIdle();
  csIdle();
  digitalWriteDirect(TP28017_PIN_RS, HIGH);
  pinModeDirect(TP28017_PIN_RD, OUTPUT);
  pinModeDirect(TP28017_PIN_WR, OUTPUT);
  pinModeDirect(TP28017_PIN_CD, OUTPUT);
  pinModeDirect(TP28017_PIN_CS, OUTPUT);
  pinModeDirect(TP28017_PIN_RS, OUTPUT);
#endif
#else
  rdIdle();
  wrIdle();
  csIdle();
  digitalWrite(_rsPin, HIGH);
  pinMode(_rdPin, OUTPUT);
  pinMode(_wrPin, OUTPUT);
  pinMode(_cdPin, OUTPUT);
  pinMode(_csPin, OUTPUT);
  pinMode(_rsPin, OUTPUT);
#endif

  setWriteDir();

  return reset(rotate180);
}

bool MxClassName::reset(bool rotate180)
{
  rdIdle();
  wrIdle();
  csIdle();

#ifdef MxPinFixed
  digitalWriteDirect(TP28017_PIN_RS, LOW); delayDirect(2);
  digitalWriteDirect(TP28017_PIN_RS, HIGH); delayDirect(120);
#else
  digitalWrite(_rsPin, LOW); delayDirect(2);
  digitalWrite(_rsPin, HIGH); delayDirect(120);
#endif

  csActive();

  command(ILI9341_NOP); // sync bytes
  wrStrobeCmd(); wrStrobeCmd(); wrStrobeCmd();

  command(ILI9341_SOFTWARE_RESET); delayDirect(5);
  command(ILI9341_DISPLAY_OFF);
  command(ILI9341_POWER_CONTROL_1, B100011);                // GVDD: 4.60 V
  command(ILI9341_POWER_CONTROL_2, 0x10 | B000);            // step-up factor
  command(ILI9341_VCOM_CONTROL_1, B0101011, B0101011);      // VMH: 3.775 V, VML: -1.425 V
  command(ILI9341_VCOM_CONTROL_2, B11000000);               // VMH/VML Offset = 0
#ifdef MxHorizontal
  command(ILI9341_MEMORY_ACESS_CONTROL, rotate180 ? ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR : ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR);
#else
  command(ILI9341_MEMORY_ACESS_CONTROL, rotate180 ? ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR : ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR);
#endif
  command(ILI9341_PIXEL_FORMAT_SET, B01010000 | B00000101); // 16 Bit / 16 Bit
  //command(ILI9341_FRAME_RATE_CONTROL, B00, B11011);       // 70 Hz (default)
  command(ILI9341_FRAME_RATE_CONTROL, B00, B11001);         // 76 Hz
  //command(ILI9341_FRAME_RATE_CONTROL, B00, B11111);         // 60 Hz

  command(ILI9341_ENTRY_MODE_SET, B0111);                   // disable low voltage detection

  command(ILI9341_SLEEP_OUT); delayDirect(5);
  command(ILI9341_DISPLAY_ON);

  return true;
}

#if defined(MxCompact)
#define setWindowInline(x1, y1, x2, y2)              \
{                                                    \
  command16(ILI9341_COLUMN_ADDRESS_SET, (x1), (x2)); \
  command16(ILI9341_PAGE_ADDRESS_SET, (y1), (y2));   \
  command(ILI9341_MEMORY_WRITE);                     \
}
#else
#ifdef MxHorizontal
#define setWindowInline(x1, y1, x2, y2)                                             \
{                                                                                   \
  commandInline(ILI9341_COLUMN_ADDRESS_SET);                                        \
  write8inline((x1) >> 8); wrStrobeData(); write8inline((x1)); wrStrobeData();      \
  write8inline((x2) >> 8); wrStrobeData(); write8inline((x2)); wrStrobeData();      \
  commandInline(ILI9341_PAGE_ADDRESS_SET);                                          \
  write8inline(0); wrStrobeData(); write8inline((y1)); wrStrobeData();              \
  write8inline(0); wrStrobeData(); write8inline((y2)); wrStrobeData();              \
  commandInline(ILI9341_MEMORY_WRITE);                                              \
}
#else
#define setWindowInline(x1, y1, x2, y2)                                             \
{                                                                                   \
  commandInline(ILI9341_COLUMN_ADDRESS_SET);                                        \
  write8inline(0); wrStrobeData(); write8inline((x1)); wrStrobeData();              \
  write8inline(0); wrStrobeData(); write8inline((x2)); wrStrobeData();              \
  commandInline(ILI9341_PAGE_ADDRESS_SET);                                          \
  write8inline((y1) >> 8); wrStrobeData(); write8inline((y1)); wrStrobeData();      \
  write8inline((y2) >> 8); wrStrobeData(); write8inline((y2)); wrStrobeData();      \
  commandInline(ILI9341_MEMORY_WRITE);                                              \
}
#endif
#endif

void MxClassName::setWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  setWindowInline(x1, y1, x2, y2);
}

void MxClassName::fillWindow(uint16_t color, uint16_t len)
{
  uint8_t hi = color >> 8;
  uint8_t lo = color;
#if defined(MxCompact) || defined(MxReference)
  for (uint16_t i = 0; i < len; i++)
  {
#if defined(MxMinimal) || defined(MxReference)
    write8(hi); wrStrobeData(); write8(lo); wrStrobeData();
#else
    write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
#endif
  }
#else
  write8(hi); wrStrobeData();
  write8(lo); wrStrobeData();
  if (hi == lo)
  {
#ifdef MxFast
    uint16_t i = 32;
    for (; i < len; i += 32)
    {
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
    }
    i -= 31;
#else
    uint16_t i = 8;
    for (; i < len; i += 8)
    {
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
      wrStrobeData(); wrStrobeData(); wrStrobeData(); wrStrobeData();
    }
    i -= 7;
#endif
    for (; i < len; i++)
    {
      wrStrobeData(); wrStrobeData();
    }
  }
  else
  {
#ifdef MxFast
    uint16_t i = 32;
    for (; i < len; i += 32)
    {
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
    }
    i -= 31;
#else
    uint16_t i = 8;
    for (; i < len; i += 8)
    {
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData(); write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
    }
    i -= 7;
#endif
    for (; i < len; i++)
    {
      write8inline(hi); wrStrobeData(); write8inline(lo); wrStrobeData();
    }
  }
#endif
}

void MxClassName::enableTrueColor()
{
  command(ILI9341_PIXEL_FORMAT_SET, B01100000 | B00000110); // 18 Bit / 18 Bit
}

void MxClassName::disableTrueColor()
{
  command(ILI9341_PIXEL_FORMAT_SET, B01010000 | B00000101); // 16 Bit / 16 Bit
}

void MxClassName::fillWindowPixel(uint16_t color)
{
#if defined(MxMinimal) || defined(MxReference)
  write8(color >> 8); wrStrobeData(); write8(color); wrStrobeData();
#else
  write8inline(color >> 8); wrStrobeData(); write8inline(color); wrStrobeData();
#endif
}

void MxClassName::fillWindowPixelTrue(uint8_t colorR, uint8_t colorG, uint8_t colorB, uint8_t x, uint8_t y)
{
  uint8_t threshold = (x & 1) != 0 ? (y & 1) != 0 ? (uint8_t)1 : (uint8_t)2 : (y & 1) != 0 ? (uint8_t)3 : (uint8_t)0;
  uint8_t r = (uint8_t)(colorR - threshold); if (r > colorR) r = colorR;
  uint8_t g = (uint8_t)(colorG - threshold); if (g > colorG) g = colorG;
  uint8_t b = (uint8_t)(colorB - threshold); if (b > colorB) b = colorB;
#if defined(MxMinimal) || defined(MxReference)
  write8(r); wrStrobeData(); write8(g); wrStrobeData(); write8(b); wrStrobeData();
#else
  write8inline(r); wrStrobeData(); write8inline(g); wrStrobeData(); write8inline(b); wrStrobeData();
#endif
}

void MxClassName::fillWindowLineTrue(uint8_t colorR, uint8_t colorG, uint8_t colorB, uint8_t x, uint8_t y, bool horizontal, uint16_t len)
{
  uint8_t threshold1 = (x & 1) != 0 ? (y & 1) != 0 ? (uint8_t)1 : (uint8_t)2 : (y & 1) != 0 ? (uint8_t)3 : (uint8_t)0;
  if (horizontal) x++; else y++;
  uint8_t threshold2 = (x & 1) != 0 ? (y & 1) != 0 ? (uint8_t)1 : (uint8_t)2 : (y & 1) != 0 ? (uint8_t)3 : (uint8_t)0;
  uint8_t r1 = (uint8_t)(colorR - threshold1); if (r1 > colorR) r1 = colorR;
  uint8_t g1 = (uint8_t)(colorG - threshold1); if (g1 > colorG) g1 = colorG;
  uint8_t b1 = (uint8_t)(colorB - threshold1); if (b1 > colorB) b1 = colorB;
  uint8_t r2 = (uint8_t)(colorR - threshold2); if (r2 > colorR) r2 = colorR;
  uint8_t g2 = (uint8_t)(colorG - threshold2); if (g2 > colorG) g2 = colorG;
  uint8_t b2 = (uint8_t)(colorB - threshold2); if (b2 > colorB) b2 = colorB;
#if defined(MxMinimal) || defined(MxReference)
  for (uint16_t i = 0; i < len; i++)
  {
    if (i & 1)
    {
      write8(r2); wrStrobeData(); write8(g2); wrStrobeData(); write8(b2); wrStrobeData();
    }
    else
    {
      write8(r1); wrStrobeData(); write8(g1); wrStrobeData(); write8(b1); wrStrobeData();
    }
  }
#else
#ifdef MxCompact
  uint16_t i = 0;
#else
  uint16_t i = 7;
  for (; i < len; i += 8)
  {
    write8inline(r1); wrStrobeData(); write8inline(g1); wrStrobeData(); write8inline(b1); wrStrobeData();
    write8inline(r2); wrStrobeData(); write8inline(g2); wrStrobeData(); write8inline(b2); wrStrobeData();
    write8inline(r1); wrStrobeData(); write8inline(g1); wrStrobeData(); write8inline(b1); wrStrobeData();
    write8inline(r2); wrStrobeData(); write8inline(g2); wrStrobeData(); write8inline(b2); wrStrobeData();
    write8inline(r1); wrStrobeData(); write8inline(g1); wrStrobeData(); write8inline(b1); wrStrobeData();
    write8inline(r2); wrStrobeData(); write8inline(g2); wrStrobeData(); write8inline(b2); wrStrobeData();
    write8inline(r1); wrStrobeData(); write8inline(g1); wrStrobeData(); write8inline(b1); wrStrobeData();
    write8inline(r2); wrStrobeData(); write8inline(g2); wrStrobeData(); write8inline(b2); wrStrobeData();
  }
  i -= 7;
#endif
  for (; i < len; i++)
  {
    if (i & 1)
    {
      write8inline(r2); wrStrobeData(); write8inline(g2); wrStrobeData(); write8inline(b2); wrStrobeData();
    }
    else
    {
      write8inline(r1); wrStrobeData(); write8inline(g1); wrStrobeData(); write8inline(b1); wrStrobeData();
    }
  }
#endif
}

// --- public methods ---

uint16_t MxClassName::width()
{
  return MxDisplayWidth;
}

uint16_t MxClassName::height()
{
  return MxDisplayHeight;
}

void MxClassName::clear(uint16_t color = 0x0000)
{
  setWindow(0, 0, MxDisplayWidth - 1, MxDisplayHeight - 1);
  fillWindow(color, MxDisplayWidth / 2 * MxDisplayHeight);
  fillWindow(color, MxDisplayWidth / 2 * MxDisplayHeight);
}

void MxClassName::clearTrue(uint8_t colorR = 0, uint8_t colorG = 0, uint8_t colorB = 0)
{
  fillRectTrue(0, 0, MxDisplayWidth, MxDisplayHeight, colorR, colorG, colorB);
}

void MxClassName::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((uint16_t)x >= MxDisplayWidth || (uint16_t)y >= MxDisplayHeight) return;
#if defined(MxMinimal) || defined(MxReference)
  setWindow((uint16_t)x, (uint16_t)y, (uint16_t)x, (uint16_t)y);
  write8(color >> 8); wrStrobeData(); write8(color); wrStrobeData();
#else
#ifdef MxFast
  setWindowInline((uint16_t)x, (uint16_t)y, (uint16_t)x, (uint16_t)y);
#else
  setWindow((uint16_t)x, (uint16_t)y, (uint16_t)x, (uint16_t)y);
#endif
  write8inline(color >> 8); wrStrobeData(); write8inline(color); wrStrobeData();
#endif
}

void MxClassName::drawPixelTrue(int16_t x, int16_t y, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  if ((uint16_t)x >= MxDisplayWidth || (uint16_t)y >= MxDisplayHeight) return;
#ifdef MxFast
  setWindowInline((uint16_t)x, (uint16_t)y, (uint16_t)x, (uint16_t)y);
#else
  setWindow((uint16_t)x, (uint16_t)y, (uint16_t)x, (uint16_t)y);
#endif
  fillWindowPixelTrue(colorR, colorG, colorB, x, y);
}

void MxClassName::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
#ifndef MxMinimal
  if ((uint16_t)x1 < MxDisplayWidth && (uint16_t)y1 < MxDisplayHeight && (uint16_t)x2 < MxDisplayWidth && (uint16_t)y2 < MxDisplayHeight)
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
#ifdef MxFast
      setWindowInline(y1, x1, y1, MxDisplayHeight - 1);
#else
      setWindow(y1, x1, y1, MxDisplayHeight - 1);
#endif
      for (; x1 <= x2; x1++)
      {
#ifdef MxCompact
        write8(color >> 8); wrStrobeData(); write8(color); wrStrobeData();
#else
        write8inline(color >> 8); wrStrobeData(); write8inline(color); wrStrobeData();
#endif
        err -= dy;
        if (err < 0)
        {
          y1 += ystep;
          err += dx;
#ifdef MxFast
          setWindowInline(y1, x1 + 1, y1, MxDisplayHeight - 1);
#else
          setWindow(y1, x1 + 1, y1, MxDisplayHeight - 1);
#endif
        }
      }
    }
    else
    {
#ifdef MxFast
      setWindowInline(x1, y1, MxDisplayWidth - 1, y1);
#else
      setWindow(x1, y1, MxDisplayWidth - 1, y1);
#endif
      for (; x1 <= x2; x1++)
      {
#ifdef MxCompact
        write8(color >> 8); wrStrobeData(); write8(color); wrStrobeData();
#else
        write8inline(color >> 8); wrStrobeData(); write8inline(color); wrStrobeData();
#endif
        err -= dy;
        if (err < 0)
        {
          y1 += ystep;
          err += dx;
#ifdef MxFast
          setWindowInline(x1 + 1, y1, MxDisplayWidth - 1, y1);
#else
          setWindow(x1 + 1, y1, MxDisplayWidth - 1, y1);
#endif
        }
      }
    }
  }
  else
#endif
  {
    drawLineSafe(x1, y1, x2, y2, color);
  }
}

void MxClassName::drawLineTrue(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
#ifndef MxMinimal
  if ((uint16_t)x1 < MxDisplayWidth && (uint16_t)y1 < MxDisplayHeight && (uint16_t)x2 < MxDisplayWidth && (uint16_t)y2 < MxDisplayHeight)
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
#ifdef MxFast
      setWindowInline(y1, x1, y1, MxDisplayHeight - 1);
#else
      setWindow(y1, x1, y1, MxDisplayHeight - 1);
#endif
      for (; x1 <= x2; x1++)
      {
        fillWindowPixelTrue(colorR, colorG, colorB, y1, x1);
        err -= dy;
        if (err < 0)
        {
          y1 += ystep;
          err += dx;
#ifdef MxFast
          setWindowInline(y1, x1 + 1, y1, MxDisplayHeight - 1);
#else
          setWindow(y1, x1 + 1, y1, MxDisplayHeight - 1);
#endif
        }
      }
    }
    else
    {
#ifdef MxFast
      setWindowInline(x1, y1, MxDisplayWidth - 1, y1);
#else
      setWindow(x1, y1, MxDisplayWidth - 1, y1);
#endif
      for (; x1 <= x2; x1++)
      {
        fillWindowPixelTrue(colorR, colorG, colorB, x1, y1);
        err -= dy;
        if (err < 0)
        {
          y1 += ystep;
          err += dx;
#ifdef MxFast
          setWindowInline(x1 + 1, y1, MxDisplayWidth - 1, y1);
#else
          setWindow(x1 + 1, y1, MxDisplayWidth - 1, y1);
#endif
        }
      }
    }
  }
  else
#endif
  {
    drawLineTrueSafe(x1, y1, x2, y2, colorR, colorG, colorB);
  }
}

void MxClassName::drawLineSafe(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
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

void MxClassName::drawLineTrueSafe(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t colorR, uint8_t colorG, uint8_t colorB)
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

void MxClassName::drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t dx = 1;
  int16_t dy = -2 * r;
  int16_t cx = 0;
  int16_t cy = r;

  drawPixel(x, y + r, color);
  drawPixel(x, y - r, color);
  drawPixel(x + r, y, color);
  drawPixel(x - r, y, color);

  while (cx < cy)
  {
    if (f >= 0)
    {
      cy--;
      dy += 2;
      f += dy;
    }
    cx++;
    dx += 2;
    f += dx;

    drawPixel(x + cx, y + cy, color);
    drawPixel(x - cx, y + cy, color);
    drawPixel(x + cx, y - cy, color);
    drawPixel(x - cx, y - cy, color);
    drawPixel(x + cy, y + cx, color);
    drawPixel(x - cy, y + cx, color);
    drawPixel(x + cy, y - cx, color);
    drawPixel(x - cy, y - cx, color);
  }
}

void MxClassName::drawCircleTrue(int16_t x, int16_t y, int16_t r, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  int16_t f = 1 - r;
  int16_t dx = 1;
  int16_t dy = -2 * r;
  int16_t cx = 0;
  int16_t cy = r;

  drawPixelTrue(x, y + r, colorR, colorG, colorB);
  drawPixelTrue(x, y - r, colorR, colorG, colorB);
  drawPixelTrue(x + r, y, colorR, colorG, colorB);
  drawPixelTrue(x - r, y, colorR, colorG, colorB);

  while (cx < cy)
  {
    if (f >= 0)
    {
      cy--;
      dy += 2;
      f += dy;
    }
    cx++;
    dx += 2;
    f += dx;

    drawPixelTrue(x + cx, y + cy, colorR, colorG, colorB);
    drawPixelTrue(x - cx, y + cy, colorR, colorG, colorB);
    drawPixelTrue(x + cx, y - cy, colorR, colorG, colorB);
    drawPixelTrue(x - cx, y - cy, colorR, colorG, colorB);
    drawPixelTrue(x + cy, y + cx, colorR, colorG, colorB);
    drawPixelTrue(x - cy, y + cx, colorR, colorG, colorB);
    drawPixelTrue(x + cy, y - cx, colorR, colorG, colorB);
    drawPixelTrue(x - cy, y - cx, colorR, colorG, colorB);
  }
}

void MxClassName::drawCircleHelper(int16_t x, int16_t y, int16_t r, uint8_t corner, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t dx = 1;
  int16_t dy = -2 * r;
  int16_t cx = 0;
  int16_t cy = r;

  while (cx < cy)
  {
    if (f >= 0)
    {
      cy--;
      dy += 2;
      f += dy;
    }
    cx++;
    dx += 2;
    f += dx;
    if (corner & 0x4)
    {
      drawPixel(x + cx, y + cy, color);
      drawPixel(x + cy, y + cx, color);
    }
    if (corner & 0x2)
    {
      drawPixel(x + cx, y - cy, color);
      drawPixel(x + cy, y - cx, color);
    }
    if (corner & 0x8)
    {
      drawPixel(x - cy, y + cx, color);
      drawPixel(x - cx, y + cy, color);
    }
    if (corner & 0x1)
    {
      drawPixel(x - cy, y - cx, color);
      drawPixel(x - cx, y - cy, color);
    }
  }
}

void MxClassName::drawCircleHelperTrue(int16_t x, int16_t y, int16_t r, uint8_t corner, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  int16_t f = 1 - r;
  int16_t dx = 1;
  int16_t dy = -2 * r;
  int16_t cx = 0;
  int16_t cy = r;

  while (cx < cy)
  {
    if (f >= 0)
    {
      cy--;
      dy += 2;
      f += dy;
    }
    cx++;
    dx += 2;
    f += dx;
    if (corner & 0x4)
    {
      drawPixelTrue(x + cx, y + cy, colorR, colorG, colorB);
      drawPixelTrue(x + cy, y + cx, colorR, colorG, colorB);
    }
    if (corner & 0x2)
    {
      drawPixelTrue(x + cx, y - cy, colorR, colorG, colorB);
      drawPixelTrue(x + cy, y - cx, colorR, colorG, colorB);
    }
    if (corner & 0x8)
    {
      drawPixelTrue(x - cy, y + cx, colorR, colorG, colorB);
      drawPixelTrue(x - cx, y + cy, colorR, colorG, colorB);
    }
    if (corner & 0x1)
    {
      drawPixelTrue(x - cy, y - cx, colorR, colorG, colorB);
      drawPixelTrue(x - cx, y - cy, colorR, colorG, colorB);
    }
  }
}

void MxClassName::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color)
{
  drawLineV(x, y - r, 2 * r + 1, color);
  fillCircleHelper(x, y, r, 3, 0, color);
}

void MxClassName::fillCircleTrue(int16_t x, int16_t y, int16_t r, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  drawLineVTrue(x, y - r, 2 * r + 1, colorR, colorG, colorB);
  fillCircleHelperTrue(x, y, r, 3, 0, colorR, colorG, colorB);
}

void MxClassName::fillCircleHelper(int16_t x, int16_t y, int16_t r, uint8_t corner, int16_t delta, uint16_t color)
{
  int16_t f = 1 - r;
  int16_t dx = 1;
  int16_t dy = -2 * r;
  int16_t cx = 0;
  int16_t cy = r;
  int16_t px = cx;
  int16_t py = cy;

  delta++;

  while (cx < cy)
  {
    if (f >= 0)
    {
      cy--;
      dy += 2;
      f += dy;
    }
    cx++;
    dx += 2;
    f += dx;
    if (cx < (cy + 1))
    {
      if (corner & 1) drawLineV(x + cx, y - cy, 2 * cy + delta, color);
      if (corner & 2) drawLineV(x - cx, y - cy, 2 * cy + delta, color);
    }
    if (cy != py)
    {
      if (corner & 1) drawLineV(x + py, y - px, 2 * px + delta, color);
      if (corner & 2) drawLineV(x - py, y - px, 2 * px + delta, color);
      py = cy;
    }
    px = cx;
  }
}

void MxClassName::fillCircleHelperTrue(int16_t x, int16_t y, int16_t r, uint8_t corner, int16_t delta, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  int16_t f = 1 - r;
  int16_t dx = 1;
  int16_t dy = -2 * r;
  int16_t cx = 0;
  int16_t cy = r;
  int16_t px = cx;
  int16_t py = cy;

  delta++;

  while (cx < cy)
  {
    if (f >= 0)
    {
      cy--;
      dy += 2;
      f += dy;
    }
    cx++;
    dx += 2;
    f += dx;
    if (cx < (cy + 1))
    {
      if (corner & 1) drawLineVTrue(x + cx, y - cy, 2 * cy + delta, colorR, colorG, colorB);
      if (corner & 2) drawLineVTrue(x - cx, y - cy, 2 * cy + delta, colorR, colorG, colorB);
    }
    if (cy != py)
    {
      if (corner & 1) drawLineVTrue(x + py, y - px, 2 * px + delta, colorR, colorG, colorB);
      if (corner & 2) drawLineVTrue(x - py, y - px, 2 * px + delta, colorR, colorG, colorB);
      py = cy;
    }
    px = cx;
  }
}

void MxClassName::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  drawLineH(x, y, w, color);
  drawLineH(x, y + h - 1, w, color);
  drawLineV(x, y, h, color);
  drawLineV(x + w - 1, y, h, color);
}

void MxClassName::drawRectTrue(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  drawLineHTrue(x, y, w, colorR, colorG, colorB);
  drawLineHTrue(x, y + h - 1, w, colorR, colorG, colorB);
  drawLineVTrue(x, y, h, colorR, colorG, colorB);
  drawLineVTrue(x + w - 1, y, h, colorR, colorG, colorB);
}

void MxClassName::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  int16_t max_radius = ((w < h) ? w : h) / 2;
  if (r > max_radius) r = max_radius;
  drawLineH(x + r, y, w - 2 * r, color);
  drawLineH(x + r, y + h - 1, w - 2 * r, color);
  drawLineV(x, y + r, h - 2 * r, color);
  drawLineV(x + w - 1, y + r, h - 2 * r, color);
  drawCircleHelper(x + r, y + r, r, 1, color);
  drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
  drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
  drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

void MxClassName::drawRoundRectTrue(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  int16_t max_radius = ((w < h) ? w : h) / 2;
  if (r > max_radius) r = max_radius;
  drawLineHTrue(x + r, y, w - 2 * r, colorR, colorG, colorB);
  drawLineHTrue(x + r, y + h - 1, w - 2 * r, colorR, colorG, colorB);
  drawLineVTrue(x, y + r, h - 2 * r, colorR, colorG, colorB);
  drawLineVTrue(x + w - 1, y + r, h - 2 * r, colorR, colorG, colorB);
  drawCircleHelperTrue(x + r, y + r, r, 1, colorR, colorG, colorB);
  drawCircleHelperTrue(x + w - r - 1, y + r, r, 2, colorR, colorG, colorB);
  drawCircleHelperTrue(x + w - r - 1, y + h - r - 1, r, 4, colorR, colorG, colorB);
  drawCircleHelperTrue(x + r, y + h - r - 1, r, 8, colorR, colorG, colorB);
}

void MxClassName::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color)
{
  int16_t max_radius = ((w < h) ? w : h) / 2;
  if (r > max_radius) r = max_radius;
  fillRect(x + r, y, w - 2 * r, h, color);
  fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
  fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

void MxClassName::fillRoundRectTrue(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  int16_t max_radius = ((w < h) ? w : h) / 2;
  if (r > max_radius) r = max_radius;
  fillRectTrue(x + r, y, w - 2 * r, h, colorR, colorG, colorB);
  fillCircleHelperTrue(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, colorR, colorG, colorB);
  fillCircleHelperTrue(x + r, y + r, r, 2, h - 2 * r - 1, colorR, colorG, colorB);
}

void MxClassName::drawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color)
{
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x3, y3, color);
  drawLine(x3, y3, x1, y1, color);
}

void MxClassName::drawTriangleTrue(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  drawLineTrue(x1, y1, x2, y2, colorR, colorG, colorB);
  drawLineTrue(x2, y2, x3, y3, colorR, colorG, colorB);
  drawLineTrue(x3, y3, x1, y1, colorR, colorG, colorB);
}

void MxClassName::fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color)
{
  int16_t a, b, y, last;

  if (y1 > y2)
  {
    swapInt(y1, y2);
    swapInt(x1, x2);
  }
  if (y2 > y3)
  {
    swapInt(y3, y2);
    swapInt(x3, x2);
  }
  if (y1 > y2)
  {
    swapInt(y1, y2);
    swapInt(x1, x2);
  }

  if (y1 == y3)
  {
    a = b = x1;
    if (x2 < a) a = x2; else if (x2 > b) b = x2;
    if (x3 < a) a = x3; else if (x3 > b) b = x3;
    drawLineH(a, y1, b - a + 1, color);
    return;
  }

  int16_t dx01 = x2 - x1, dy01 = y2 - y1, dx02 = x3 - x1, dy02 = y3 - y1, dx12 = x3 - x2, dy12 = y3 - y2;
  int32_t sa = 0, sb = 0;

  if (y2 == y3) last = y2; else last = y2 - 1;

  for (y = y1; y <= last; y++)
  {
    a = x1 + sa / dy01;
    b = x1 + sb / dy02;
    sa += dx01;
    sb += dx02;
    if (a > b) swapInt(a, b);
    drawLineH(a, y, b - a + 1, color);
  }

  sa = (int32_t)dx12 * (y - y2);
  sb = (int32_t)dx02 * (y - y1);
  for (; y <= y3; y++)
  {
    a = x2 + sa / dy12;
    b = x1 + sb / dy02;
    sa += dx12;
    sb += dx02;
    if (a > b) swapInt(a, b);
    drawLineH(a, y, b - a + 1, color);
  }
}

void MxClassName::fillTriangleTrue(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  int16_t a, b, y, last;

  if (y1 > y2)
  {
    swapInt(y1, y2);
    swapInt(x1, x2);
  }
  if (y2 > y3)
  {
    swapInt(y3, y2);
    swapInt(x3, x2);
  }
  if (y1 > y2)
  {
    swapInt(y1, y2);
    swapInt(x1, x2);
  }

  if (y1 == y3)
  {
    a = b = x1;
    if (x2 < a) a = x2; else if (x2 > b) b = x2;
    if (x3 < a) a = x3; else if (x3 > b) b = x3;
    drawLineHTrue(a, y1, b - a + 1, colorR, colorG, colorB);
    return;
  }

  int16_t dx01 = x2 - x1, dy01 = y2 - y1, dx02 = x3 - x1, dy02 = y3 - y1, dx12 = x3 - x2, dy12 = y3 - y2;
  int32_t sa = 0, sb = 0;

  if (y2 == y3) last = y2; else last = y2 - 1;

  for (y = y1; y <= last; y++)
  {
    a = x1 + sa / dy01;
    b = x1 + sb / dy02;
    sa += dx01;
    sb += dx02;
    if (a > b) swapInt(a, b);
    drawLineHTrue(a, y, b - a + 1, colorR, colorG, colorB);
  }

  sa = (int32_t)dx12 * (y - y2);
  sb = (int32_t)dx02 * (y - y1);
  for (; y <= y3; y++)
  {
    a = x2 + sa / dy12;
    b = x1 + sb / dy02;
    sa += dx12;
    sb += dx02;
    if (a > b) swapInt(a, b);
    drawLineHTrue(a, y, b - a + 1, colorR, colorG, colorB);
  }
}

void MxClassName::drawLineV(int16_t x, int16_t y, int16_t height, uint16_t color)
{
  if ((uint16_t)x >= MxDisplayWidth) return;
  if (y < 0) { height += y; y = 0; }
  if (y + height > MxDisplayHeight) height = MxDisplayHeight - y;
  if (height <= 0) return;
  setWindow(x, y, x, y + height - 1);
  fillWindow(color, height);
}

void MxClassName::drawLineVTrue(int16_t x, int16_t y, int16_t height, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  if ((uint16_t)x >= MxDisplayWidth) return;
  if (y < 0) { height += y; y = 0; }
  if (y + height > MxDisplayHeight) height = MxDisplayHeight - y;
  if (height <= 0) return;
  setWindow(x, y, x, y + height - 1);
  fillWindowLineTrue(colorR, colorG, colorB, x, y, false, height);
}

void MxClassName::drawLineH(int16_t x, int16_t y, int16_t width, uint16_t color)
{
  if ((uint16_t)y >= MxDisplayHeight) return;
  if (x < 0) { width += x; x = 0; }
  if (x + width > MxDisplayWidth) width = MxDisplayWidth - x;
  if (width <= 0) return;
  setWindow(x, y, x + width - 1, y);
  fillWindow(color, width);
}

void MxClassName::drawLineHTrue(int16_t x, int16_t y, int16_t width, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  if ((uint16_t)y >= MxDisplayHeight) return;
  if (x < 0) { width += x; x = 0; }
  if (x + width > MxDisplayWidth) width = MxDisplayWidth - x;
  if (width <= 0) return;
  setWindow(x, y, x + width - 1, y);
  fillWindowLineTrue(colorR, colorG, colorB, x, y, true, width);
}

void MxClassName::fillRect(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
{
  if (x < 0) { width += x; x = 0; }
  if (y < 0) { height += y; y = 0; }
  if (x + width > MxDisplayWidth) width = MxDisplayWidth - x;
  if (y + height > MxDisplayHeight) height = MxDisplayHeight - y;
  if (width <= 0 || height <= 0) return;
  setWindow(x, y, x + width - 1, y + height - 1);
  if (width < 205 || height < 205 || (width < 256 && height < 256))
  {
    fillWindow(color, width * height);
    return;
  }
  uint32_t pixel = (uint32_t)width * (uint32_t)height;
  if (pixel > 65280)
  {
    fillWindow(color, 65280);
    pixel -= 65280;
  }
  fillWindow(color, pixel);
}

void MxClassName::fillRectTrue(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
  if (x < 0) { width += x; x = 0; }
  if (y < 0) { height += y; y = 0; }
  if (x + width > MxDisplayWidth) width = MxDisplayWidth - x;
  if (y + height > MxDisplayHeight) height = MxDisplayHeight - y;
  if (width <= 0 || height <= 0) return;
  setWindow(x, y, x + width - 1, y + height - 1);
  height += y & 1;
#ifdef MxHorizontal
  for (uint8_t cy = y & 1; cy < (uint8_t)height; cy++)
  {
#else
  for (uint16_t cy = y & 1; cy < height; cy++)
  {
#endif
    fillWindowLineTrue(colorR, colorG, colorB, x, cy, true, width);
  }
}

uint16_t MxClassName::getScanline()
{
  command(ILI9341_GET_SCANLINE);
  wrIdle();
  setReadDir();
  //delayMicroseconds(3);
  rdActive(); rdIdle();
  rdActive();
  uint8_t hi = read8();
  rdIdle();
  rdActive();
  uint8_t lo = read8();
  rdIdle();
  setWriteDir();
  return (hi << 8) | lo;
}

void MxClassName::vSync(uint16_t line = 260)
{
  for (; getScanline() > line;) {}
  for (; getScanline() <= line;) {}
}

void MxClassName::setScrollPosition(uint16_t posY)
{
  command(ILI9341_VERTICAL_SCROLLING_START_ADDRESS, posY >> 8, posY);
}
