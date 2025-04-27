#include "HomeSpan.h"
#include <Wire.h>
#include <BH1750.h>

#define MAX_CURRENT 2.0
TaskHandle_t CurrentMonitorTaskHandle; // TODO

#define NPIXELS 60
Pixel pixel(16, "GRBW");
Pixel::Color colorRGB; 
Pixel::Color colorWC;

BH1750 lightMeter(0x23);

#define NIGHT_LIGHT_THRESHOLD 100
#define NIGHT_LIGHT_BRIGHTNESS 32
TaskHandle_t NightLightTaskHandle;
bool areLightsOff(){
  return (colorRGB==Pixel::RGB(0,0,0,0) && colorWC==Pixel::RGB(0,0,0,0));
}
void NightLightTask( void * parameter) {
  for(;;) {
    int lux = lightMeter.readLightLevel();
    Serial.printf("Lux: %d\n", lux);
    if (areLightsOff()) {
      if (lux > NIGHT_LIGHT_THRESHOLD) {
        pixel.set(Pixel::RGB(0, 0, 0, 0), NPIXELS);
      } else {
        pixel.set(Pixel::RGB(0, 0, 0, NIGHT_LIGHT_BRIGHTNESS), NPIXELS);
      }
    }
    vTaskDelay(1000);
  }
}
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
    pixel.set(colorRGB+colorWC,NPIXELS);
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
    pixel.set(colorRGB+colorWC,NPIXELS);
    return(true);
  }
};

// TODO: fork Pixel class to add setting each pixel to a different color; ideally make a PR to HomeSpan to get some fields as protected

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  lightMeter.begin();
 
  homeSpan.begin(Category::Lighting,"DoorLedStrip","DoorLedStrip","v0.1");
  SPAN_ACCESSORY();
  SPAN_ACCESSORY("DoorLedStrip White");
    new NeoPixel_W();
  SPAN_ACCESSORY("DoorLedStrip RGB");
    new NeoPixel_RGB();
  
  xTaskCreate(
    NightLightTask,
    "NightLightTask",
    10000,
    NULL,
    1,
    &NightLightTaskHandle);
  delay(500); 
}
void loop() {
  homeSpan.poll();
}
