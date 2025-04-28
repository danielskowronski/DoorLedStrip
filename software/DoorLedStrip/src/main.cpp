#include "Arduino.h"
#include <Wire.h>
#include "HomeSpan.h"
#include "Settings.h"
#include "State.h"
#include "LightMeter.h"
#include "NightLight.h"
#include "hardware.h"
#include "Pixel.h"
#include "CurrentMonitor.h"
#include "HomeSpanLightBulb.h"




void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  loadPreferences();
  lightMeterInit();
 
  homeSpan.begin(Category::Lighting,"DoorLedStrip","DoorLedStrip","v0.1"); // TODO: move to proper values
  SPAN_ACCESSORY();
  SPAN_ACCESSORY("DoorLedStrip White");
    new NeoPixel_W(BulbW);
  SPAN_ACCESSORY("DoorLedStrip RGB");
    new NeoPixel_RGB(BulbRGB);
  
  xTaskCreate(NightLightTask,"NightLightTask",10000,NULL,1,&NightLightTaskHandle);
  delay(500); 
}
void loop() {
  homeSpan.poll();
}
