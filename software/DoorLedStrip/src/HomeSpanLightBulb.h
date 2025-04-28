#pragma once
#include "Arduino.h"
#include "HomeSpan.h"

struct NeoPixel_RGB : Service::LightBulb {
  NeoPixel_RGB();
};
struct NeoPixel_W : Service::LightBulb {
  NeoPixel_W();
};
extern NeoPixel_RGB BulbRGB;
extern NeoPixel_W BulbW;