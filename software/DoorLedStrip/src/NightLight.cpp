#include "hardware.h"
#include "NightLight.h"
#include "LightMeter.h"
#include "Pixel.h"

TaskHandle_t NightLightTaskHandle;
bool areLightsOff(){
  return (colorRGB==Pixel::RGB(0,0,0,0) && colorWC==Pixel::RGB(0,0,0,0));
}
void NightLightTask( void * parameter) {
  for(;;) {
    int lux = lightMeter.readLightLevel();
    //Serial.printf("Lux: %d\n", lux);
    if (areLightsOff()) {
      if (lux > _settings.nightLight.luxThreshold) {
        pixel.set(Pixel::RGB(0, 0, 0, 0), LED_NPIXELS);
      } else {
        pixel.set(Pixel::RGB(0, 0, 0, _settings.nightLight.targetBrightness), LED_NPIXELS);
      }
    }
    vTaskDelay(1000);
  }
}