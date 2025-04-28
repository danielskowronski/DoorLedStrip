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
#include "WebServer.h"


bool systemReady = false;


void setup() {
  Serial.begin(115200);
  Serial.println("Starting DoorLedStrip");
  Wire.begin(I2C_SDA, I2C_SCL);
  loadPreferences();
  lightMeterInit();
 
  homeSpan.begin(Category::Lighting,"DoorLedStrip","DoorLedStrip","v0.1"); // TODO: move to proper values
  SPAN_ACCESSORY();
  SPAN_ACCESSORY("DoorLedStrip White");
    BulbW = new NeoPixel_W(); 
  SPAN_ACCESSORY("DoorLedStrip RGB");
    BulbRGB = new NeoPixel_RGB();
  xTaskCreate(NightLightTask,"NightLightTask",10000,NULL,1,&NightLightTaskHandle);


  delay(500); 
}
void loop() {
  homeSpan.poll();
  if (!systemReady && WiFi.status() == WL_CONNECTED) {
    systemReady = true;
    setupWebServer();  // if you have one
  }
}
