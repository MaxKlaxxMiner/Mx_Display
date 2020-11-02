#include <Mx_Display_ILI9341.h>

Mx_DisplayH_ILI9341_Fastest_NoSd_NoSerial tft(false); // remark: horizontal "DisplayH"

void setup()
{
  tft.clear(0x0000); // black
}

void loop()
{
  const uint32_t step = 3000; // 3000 milli seconds = 360 degree
  const int midX = 160;
  const int startY = 60;
  const int midY = 120;
  const int endY = 180;
  const float width = 100.0;
  const float height = 30.0;

  uint32_t time = -millis() % step;
  float rad = time * (PI * 2.0) / step;
  int x1 = (int)(sin(rad) * width);
  int x2 = (int)(sin(rad + (PI / 2)) * width);
  int y1 = (int)(cos(rad) * -height);
  int y2 = (int)(cos(rad + (PI / 2)) * -height);

  // --- draw lines light blue ---
  tft.drawLine(midX + x1, startY + y1, midX + x2, startY + y2, color565(0, 128, 255));
  tft.drawLine(midX + x2, startY + y2, midX - x1, startY - y1, color565(0, 128, 255));
  tft.drawLine(midX - x1, startY - y1, midX - x2, startY - y2, color565(0, 128, 255));
  tft.drawLine(midX - x2, startY - y2, midX + x1, startY + y1, color565(0, 128, 255));

  if (time <= step / 8 * 7 && time >= step / 8) tft.drawLine(midX + x1, startY + y1, midX + x1, endY + y1, color565(0, 128, 255));
  if (time <= step / 8 * 5 || time >= step / 8 * 7) tft.drawLine(midX + x2, startY + y2, midX + x2, endY + y2, color565(0, 128, 255));
  if (time <= step / 8 * 3 || time >= step / 8 * 5) tft.drawLine(midX - x1, startY - y1, midX - x1, endY - y1, color565(0, 128, 255));
  if (time <= step / 8 || time >= step / 8 * 3) tft.drawLine(midX - x2, startY - y2, midX - x2, endY - y2, color565(0, 128, 255));

  if (time <= step / 8 * 5 && time >= step / 8) tft.drawLine(midX + x1, endY + y1, midX + x2, endY + y2, color565(0, 128, 255));
  if (time <= step / 8 * 3 || time >= step / 8 * 7) tft.drawLine(midX + x2, endY + y2, midX - x1, endY - y1, color565(0, 128, 255));
  if (time <= step / 8 || time >= step / 8 * 5) tft.drawLine(midX - x1, endY - y1, midX - x2, endY - y2, color565(0, 128, 255));
  if (time <= step / 8 * 7 && time >= step / 8 * 3) tft.drawLine(midX - x2, endY - y2, midX + x1, endY + y1, color565(0, 128, 255));

#ifndef __AVR_ATmega2560__
  tft.vSync();
#else
  delayDirect(25); // to slow for 76 fps -> use delay
#endif
    
  // --- draw lines black (remove lines are faster than clear entire screen) ---
  tft.drawLine(midX + x1, startY + y1, midX + x2, startY + y2, 0);
  tft.drawLine(midX + x2, startY + y2, midX - x1, startY - y1, 0);
  tft.drawLine(midX - x1, startY - y1, midX - x2, startY - y2, 0);
  tft.drawLine(midX - x2, startY - y2, midX + x1, startY + y1, 0);
    
  if (time <= step / 8 * 7 && time >= step / 8) tft.drawLine(midX + x1, startY + y1, midX + x1, endY + y1, 0);
  if (time <= step / 8 * 5 || time >= step / 8 * 7) tft.drawLine(midX + x2, startY + y2, midX + x2, endY + y2, 0);
  if (time <= step / 8 * 3 || time >= step / 8 * 5) tft.drawLine(midX - x1, startY - y1, midX - x1, endY - y1, 0);
  if (time <= step / 8 || time >= step / 8 * 3) tft.drawLine(midX - x2, startY - y2, midX - x2, endY - y2, 0);
    
  if (time <= step / 8 * 5 && time >= step / 8) tft.drawLine(midX + x1, endY + y1, midX + x2, endY + y2, 0);
  if (time <= step / 8 * 3 || time >= step / 8 * 7) tft.drawLine(midX + x2, endY + y2, midX - x1, endY - y1, 0);
  if (time <= step / 8 || time >= step / 8 * 5) tft.drawLine(midX - x1, endY - y1, midX - x2, endY - y2, 0);
  if (time <= step / 8 * 7 && time >= step / 8 * 3) tft.drawLine(midX - x2, endY - y2, midX + x1, endY + y1, 0);
}
