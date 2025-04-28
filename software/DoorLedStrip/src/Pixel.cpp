#include "Pixel.h"
Pixel pixel(LED_PIN, LED_MODE);
void rgbToHsv(uint8_t r, uint8_t g, uint8_t b, float &h, float &s, float &v)
{
  float fr = r / 255.0;
  float fg = g / 255.0;
  float fb = b / 255.0;

  float maxVal = max(fr, max(fg, fb));
  float minVal = min(fr, min(fg, fb));
  float delta = maxVal - minVal;

  // Value
  v = maxVal;

  // Saturation
  if (maxVal == 0)
  {
    s = 0;
    h = 0; // undefined hue
    return;
  }
  else
  {
    s = delta / maxVal;
  }

  // Hue
  if (delta == 0)
  {
    h = 0; // undefined hue
  }
  else if (maxVal == fr)
  {
    h = 60.0 * (fmod(((fg - fb) / delta), 6));
  }
  else if (maxVal == fg)
  {
    h = 60.0 * (((fb - fr) / delta) + 2);
  }
  else if (maxVal == fb)
  {
    h = 60.0 * (((fr - fg) / delta) + 4);
  }

  if (h < 0)
  {
    h += 360.0;
  }
}
