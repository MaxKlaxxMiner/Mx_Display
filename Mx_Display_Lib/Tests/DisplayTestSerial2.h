
#include <arduino.h>
#include "../Mx_Basics.h"
#include "../Mx_Display_Serial_Ada.h"

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define color565(r, g, b) (((r) & 0xF8) << 8 | ((g) & 0xFC) << 3 | (b) >> 3)

unsigned long testFillScreen()
{
  unsigned long start = micros();
  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.fillScreen(BLACK);
  return micros() - start;
}

//unsigned long testText()
//{
//  tft.fillScreen(BLACK);
//  unsigned long start = micros();
//  tft.setCursor(0, 0);
//  tft.setTextColor(WHITE);  tft.setTextSize(1);
//  tft.println("Hello World!");
//  tft.setTextColor(YELLOW); tft.setTextSize(2);
//  tft.println(1234.56);
//  tft.setTextColor(RED);    tft.setTextSize(3);
//  tft.println(0xDEADBEEF, HEX);
//  tft.println();
//  tft.setTextColor(GREEN);
//  tft.setTextSize(5);
//  tft.println("Groop");
//  tft.setTextSize(2);
//  tft.println("I implore thee,");
//  tft.setTextSize(1);
//  tft.println("my foonting turlingdromes.");
//  tft.println("And hooptiously drangle me");
//  tft.println("with crinkly bindlewurdles,");
//  tft.println("Or I will rend thee");
//  tft.println("in the gobberwarts");
//  tft.println("with my blurglecruncheon,");
//  tft.println("see if I don't!");
//  return micros() - start;
//}
//
//unsigned long testLines(uint16_t color)
//{
//  unsigned long start, t;
//  int x1, y1, x2, y2;
//  int w = tft.width();
//  int h = tft.height();
//
//  tft.fillScreen(BLACK);
//
//  x1 = y1 = 0;
//  y2 = h - 1;
//  start = micros();
//  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
//  x2 = w - 1;
//  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
//  t = micros() - start; // fillScreen doesn't count against timing
//
//  tft.fillScreen(BLACK);
//
//  x1 = w - 1;
//  y1 = 0;
//  y2 = h - 1;
//  start = micros();
//  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
//  x2 = 0;
//  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
//  t += micros() - start;
//
//  tft.fillScreen(BLACK);
//
//  x1 = 0;
//  y1 = h - 1;
//  y2 = 0;
//  start = micros();
//  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
//  x2 = w - 1;
//  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
//  t += micros() - start;
//
//  tft.fillScreen(BLACK);
//
//  x1 = w - 1;
//  y1 = h - 1;
//  y2 = 0;
//  start = micros();
//  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
//  x2 = 0;
//  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
//
//  return micros() - start;
//}

unsigned long testFastLines(uint16_t color1, uint16_t color2)
{
  unsigned long start;
  int x, y;
  int w = tft.width();
  int h = tft.height();

  tft.fillScreen(BLACK);
  start = micros();
  for (y = 0; y < h; y += 5) tft.drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

//unsigned long testRects(uint16_t color)
//{
//  unsigned long start;
//  int n, i, i2;
//  int cx = tft.width() / 2;
//  int cy = tft.height() / 2;
//
//  tft.fillScreen(BLACK);
//  n = min(tft.width(), tft.height());
//  start = micros();
//  for (i = 2; i < n; i += 6)
//  {
//    i2 = i / 2;
//    tft.drawRect(cx - i2, cy - i2, i, i, color);
//  }
//
//  return micros() - start;
//}
//
//unsigned long testFilledRects(uint16_t color1, uint16_t color2)
//{
//  unsigned long start, t = 0;
//  int n, i, i2;
//  int cx = tft.width() / 2 - 1;
//  int cy = tft.height() / 2 - 1;
//
//  tft.fillScreen(BLACK);
//  n = min(tft.width(), tft.height());
//  for (i = n; i > 0; i -= 6)
//  {
//    i2 = i / 2;
//    start = micros();
//    tft.fillRect(cx - i2, cy - i2, i, i, color1);
//    t += micros() - start;
//    // Outlines are not included in timing results
//    tft.drawRect(cx - i2, cy - i2, i, i, color2);
//  }
//
//  return t;
//}
//
//unsigned long testFilledCircles(uint8_t radius, uint16_t color)
//{
//  unsigned long start;
//  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;
//
//  tft.fillScreen(BLACK);
//  start = micros();
//  for (x = radius; x < w; x += r2)
//  {
//    for (y = radius; y < h; y += r2)
//    {
//      tft.fillCircle(x, y, radius, color);
//    }
//  }
//
//  return micros() - start;
//}
//
//unsigned long testCircles(uint8_t radius, uint16_t color)
//{
//  unsigned long start;
//  int x, y, r2 = radius * 2;
//  int w = tft.width() + radius;
//  int h = tft.height() + radius;
//
//  // Screen is not cleared for this one -- this is
//  // intentional and does not affect the reported time.
//  start = micros();
//  for (x = 0; x < w; x += r2)
//  {
//    for (y = 0; y < h; y += r2)
//    {
//      tft.drawCircle(x, y, radius, color);
//    }
//  }
//
//  return micros() - start;
//}
//
//unsigned long testTriangles()
//{
//  unsigned long start;
//  int n, i;
//  int cx = tft.width() / 2 - 1;
//  int cy = tft.height() / 2 - 1;
//
//  tft.fillScreen(BLACK);
//  n = min(cx, cy);
//  start = micros();
//  for (i = 0; i < n; i += 5)
//  {
//    tft.drawTriangle(
//      cx, cy - i, // peak
//      cx - i, cy + i, // bottom left
//      cx + i, cy + i, // bottom right
//      color565(0, 0, i));
//  }
//
//  return micros() - start;
//}
//
//unsigned long testFilledTriangles()
//{
//  unsigned long start, t = 0;
//  int i;
//  int cx = tft.width() / 2 - 1;
//  int cy = tft.height() / 2 - 1;
//
//  tft.fillScreen(BLACK);
//  start = micros();
//  for (i = min(cx, cy); i > 10; i -= 5)
//  {
//    start = micros();
//    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color565(0, i, i));
//    t += micros() - start;
//    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i, color565(i, i, 0));
//  }
//
//  return t;
//}
//
//unsigned long testRoundRects()
//{
//  unsigned long start;
//  int w, i, i2;
//  int cx = tft.width() / 2 - 1;
//  int cy = tft.height() / 2 - 1;
//
//  tft.fillScreen(BLACK);
//  w = min(tft.width(), tft.height());
//  start = micros();
//  for (i = 0; i < w; i += 6)
//  {
//    i2 = i / 2;
//    tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, color565(i, 0, 0));
//  }
//
//  return micros() - start;
//}
//
//unsigned long testFilledRoundRects()
//{
//  unsigned long start;
//  int i, i2;
//  int cx = tft.width() / 2 - 1;
//  int cy = tft.height() / 2 - 1;
//
//  tft.fillScreen(BLACK);
//  start = micros();
//  for (i = min(tft.width(), tft.height()); i > 20; i -= 6)
//  {
//    i2 = i / 2;
//    tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, color565(0, i, 0));
//  }
//
//  return micros() - start;
//}

void setup(void)
{
  pinModeDirect(13, OUTPUT);
  digitalWriteDirect(13, LOW);
  Serial.begin(500000);

  testFillScreen();
  delay(500);

  //testText();
  //delay(3000);

  //testLines(CYAN);
  //delay(500);

  testFastLines(RED, BLUE);
  delay(500);

  //testRects(GREEN);
  //delay(500);

  //testFilledRects(YELLOW, MAGENTA);
  //delay(500);

  //testFilledCircles(10, MAGENTA);

  //testCircles(10, WHITE);
  //delay(500);

  //testTriangles();
  //delay(500);

  //testFilledTriangles();
  //delay(500);

  //testRoundRects();
  //delay(500);

  //testFilledRoundRects();
  //delay(500);
}

void loop(void)
{
  //for (uint8_t rotation = 0; rotation < 4; rotation++)
  //{
  //  tft.setRotation(rotation);
  //  testText();
  //  delay(1000);
  //  for (int x = 0; x < tft.width(); x++)
  //  {
  //    tft.drawLine(x, 0, 0, tft.height() - 1, color565(0, (uint32_t)x * 128 / tft.width(), (uint32_t)x * 256 / tft.width()));
  //  }
  //  for (int y = 0; y < tft.height(); y++)
  //  {
  //    tft.drawLine(tft.width() - 1, y, 0, tft.height() - 1, color565((uint32_t)y * 256 / tft.height(), (uint32_t)y * 128 / tft.height() + 127, 255));
  //  }
  //  delay(1000);
  //}
}
