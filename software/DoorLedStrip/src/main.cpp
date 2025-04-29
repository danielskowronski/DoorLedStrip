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
#include "API.h"
#include "OvercurrentWatchdog.h"

void statusUpdate(HS_STATUS status)
{
  Serial.printf("\n*** HOMESPAN STATUS CHANGE: %s\n", homeSpan.statusString(status));
  if (status == HS_STATUS::HS_PAIRED)
  {
    Serial.println("[INFO] HomeSpan is ready.");
    setupWebServer();
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting DoorLedStrip");
  Wire.begin(I2C_SDA, I2C_SCL);
  loadPreferences();
  lightMeterInit();
  currentMonitorInit();
  homeSpan.setStatusCallback(statusUpdate);
  homeSpan.setPortNum(PORT_HS);
  homeSpan.begin(Category::Bridges, "DoorLedStrip", "DLS", "DoorLedStrip"); // TODO: move to proper values

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new Characteristic::Name("DoorLedStrip White");
  BulbW = new NeoPixel_W();

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new Characteristic::Name("DoorLedStrip RGB");
  BulbRGB = new NeoPixel_RGB();

  Serial.println("Installing NightLightTask");
  xTaskCreate(NightLightTask, "NightLightTask", 10000, NULL, 1, &NightLightTaskHandle);
  Serial.println("Installing OvercurrentWatchdogTask");
  xTaskCreate(OvercurrentWatchdogTask, "OvercurrentWatchdogTask", 10000, NULL, 0, &OvercurrentWatchdogTaskHandle);

  Serial.println("Final sleep before loop");
  delay(500);
}

void loop()
{
  homeSpan.poll();
}

// TODO: use HomeSpan log decorators and fix output
