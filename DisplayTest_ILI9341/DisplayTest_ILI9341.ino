#include "Mx_Display_ILI9341.h"
#include "Mx_Display_Scrollable.h"
#include "Mx_Display_Console.h"
#include "Mx_Display_AdafruitWrapper.h"

Mx_DisplayV_ILI9341_Fastest tft(false);
Mx_Display_AdafruitWrapper<Mx_DisplayV_ILI9341_Fastest> ada(&tft);

void setup()
{
  pinModeDirect(LED_BUILTIN, OUTPUT);
  digitalWriteDirect(LED_BUILTIN, LOW);

  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  tft.clear(0x0000);
}

int cx = 0;
int cy = 0;

void loop()
{
  int _xp = 8;
  int _yp = A3;
  int _xm = A2;
  int _ym = 9;

  int samples[2];
  bool valid = true;

  pinMode(_yp, INPUT);
  pinMode(_ym, INPUT);
  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);

  digitalWrite(_xp, HIGH);
  digitalWrite(_xm, LOW);

  for (int i = 0; i < 2; i++)
  {
    samples[i] = analogRead(_yp);
  }

  if (samples[0] - samples[1] < -4 || samples[0] - samples[1] > 4)
  {
    valid = false;
  }
  else
  {
    samples[1] = (samples[0] + samples[1]) >> 1; // average 2 samples
  }

  int x = 1023 - samples[1];

  pinMode(_xp, INPUT);
  pinMode(_xm, INPUT);
  pinMode(_yp, OUTPUT);
  pinMode(_ym, OUTPUT);

  digitalWrite(_ym, LOW);
  digitalWrite(_yp, HIGH);

  for (int i = 0; i < 2; i++)
  {
    samples[i] = analogRead(_xm);
  }

  if (samples[0] - samples[1] < -4 || samples[0] - samples[1] > 4)
  {
    valid = false;
  }
  else
  {
    samples[1] = (samples[0] + samples[1]) >> 1; // average 2 samples
  }

  int y = 1023 - samples[1];

  pinMode(_xp, OUTPUT);
  pinMode(_yp, INPUT);

  digitalWrite(_xp, LOW);
  digitalWrite(_ym, HIGH);

  int z1 = analogRead(_xm);
  int z2 = analogRead(_yp);

  float rtouch;
  rtouch = z2;
  rtouch /= z1;
  rtouch -= 1;
  rtouch *= x;
  rtouch *= 300;
  rtouch /= 1024;
  int z = rtouch;

  if (!valid || z == 0)
  {
    x = 0;
    y = 0;
    z = 0;
  }

  pinMode(_xm, OUTPUT);
  pinMode(_yp, OUTPUT);

  if (z != 0)
  {
    tft.drawCircle(cx, cy, 50, 0x0000);
    tft.drawCircle(cx, cy, 5, 0x0000);
    const int xFrom = 910;
    const int xTo = 120;
    const int yFrom = 75;
    const int yTo = 905;
    cx = (int32_t)tft.width() - ((int32_t)(x - xTo) * (int32_t)tft.width() / (int32_t)(xFrom - xTo));
    cy = (int32_t)(y - yFrom) * (int32_t)tft.height() / (int32_t)(yTo - yFrom);
    tft.drawCircle(cx, cy, 50, color565(0, 128, 255));
    tft.drawCircle(cx, cy, 5, color565(0, 128, 255));
    if (cy >= 0 && cy < 324) tft.vSync((cy + 50) % 320);
  }
}
