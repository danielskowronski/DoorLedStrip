#include "HomeSpanLightBulb.h"
#include "hardware.h"
#include "State.h"
#include "Pixel.h"
#include "OvercurrentWatchdog.h"

NeoPixel_RGB::NeoPixel_RGB() : Service::LightBulb()
{
  V.setRange(1, 100, 1);
  update();
}
boolean NeoPixel_RGB::update()
{
  colorRGB = pixel.HSV(H.getNewVal(), S.getNewVal(), V.getNewVal() * power.getNewVal());
  double overcurrentRate = predictedOvercurrentRate(colorRGB, colorWC);
  if (overcurrentRate > 1)
  {
    colorRGB = pixel.HSV(H.getNewVal(), S.getNewVal(), V.getNewVal() / overcurrentRate * power.getNewVal());
    V.setVal(int(V.getNewVal() / overcurrentRate));
  }
  pixel.set(colorRGB + colorWC, LED_NPIXELS);
  return (true);
}
NeoPixel_W::NeoPixel_W() : Service::LightBulb()
{
  V.setRange(1, 100, 1);
  update();
}

boolean NeoPixel_W::update()
{
  colorWC = pixel.CCT(1.0e6 / 4000, V.getNewVal() * power.getNewVal());
  double overcurrentRate = predictedOvercurrentRate(colorRGB, colorWC);
  if (overcurrentRate > 1)
  {
    colorWC = pixel.CCT(1.0e6 / 4000, V.getNewVal() / overcurrentRate * power.getNewVal());
    V.setVal(int(V.getNewVal() / overcurrentRate));
  }
  pixel.set(colorRGB + colorWC, LED_NPIXELS);
  return (true);
}

NeoPixel_RGB *BulbRGB = nullptr;
NeoPixel_W *BulbW = nullptr;
