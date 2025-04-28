#include "HomeSpanLightBulb.h"
#include "hardware.h"
#include "State.h"
#include "Pixel.h"

struct NeoPixel_RGB : Service::LightBulb { 
  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{100,true};
  int nPixels;
  
  NeoPixel_RGB() : Service::LightBulb(){
    V.setRange(1,100,1);
    update();
  }

  boolean update() override {
    colorRGB=pixel.HSV(H.getNewVal(), S.getNewVal(), V.getNewVal()*power.getNewVal());
    pixel.set(colorRGB+colorWC,LED_NPIXELS);
    return(true);
  }
};
struct NeoPixel_W : Service::LightBulb {
  Characteristic::On power{0,true};
  Characteristic::Brightness V{100,true};
  
  NeoPixel_W() : Service::LightBulb(){
    V.setRange(1,100,1);
    update();
  }

  boolean update() override {
    colorWC=pixel.CCT(1.0e6/4000, V.getNewVal()*power.getNewVal());
    pixel.set(colorRGB+colorWC,LED_NPIXELS);
    return(true);
  }
};

NeoPixel_RGB BulbRGB;
NeoPixel_W BulbW;