#pragma once
#include "Arduino.h"
#include "HomeSpan.h"

struct NeoPixel_RGB : Service::LightBulb
{
  Characteristic::On power{0, true};
  Characteristic::Hue H{0, true};
  Characteristic::Saturation S{0, true};
  Characteristic::Brightness V{100, true};
  int nPixels;

  NeoPixel_RGB();
  boolean update() override;
};
struct NeoPixel_W : Service::LightBulb
{
  Characteristic::On power{0, true};
  Characteristic::Brightness V{100, true};

  NeoPixel_W();
  boolean update() override;
};
extern NeoPixel_RGB *BulbRGB;
extern NeoPixel_W *BulbW;
