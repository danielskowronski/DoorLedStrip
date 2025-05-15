#include "hardware.h"
#include "NightLight.h"
#include "LightMeter.h"
#include "Pixel.h"
#include "HomeSpan.h"
#include "MQTT.h"

TaskHandle_t NightLightTaskHandle;

bool areLightsOff()
{
  return (colorRGB == Pixel::RGB(0, 0, 0, 0) && colorWC == Pixel::RGB(0, 0, 0, 0));
}

void NightLightTask(void *parameter)
{
  for (;;)
  {
    int lux = lightMeter.readLightLevel();
    mqttPublishLightSensor(lux);
    // Serial.printf("Lux: %d\n", lux);
    if (areLightsOff())
    {
      homeSpanPAUSE;
      if (lux >= _settings.nightLight.thresholdOff)
      {
        pixel.set(Pixel::RGB(0, 0, 0, 0), LED_NPIXELS);
        pixel.set(Pixel::RGB(0, 0, 0, 0), LED_NPIXELS);
      }
      if (lux <= _settings.nightLight.thresholdOn)
      {
        pixel.set(Pixel::RGB(0, 0, 0, _settings.nightLight.targetBrightness), LED_NPIXELS);
        pixel.set(Pixel::RGB(0, 0, 0, _settings.nightLight.targetBrightness), LED_NPIXELS);
      }
      homeSpanRESUME;
    }
    vTaskDelay(1000);
  }
}
