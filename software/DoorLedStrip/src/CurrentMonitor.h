#pragma once
#include "Arduino.h"
#include "hardware.h"
#include <Adafruit_INA219.h>

struct CurrentMonitorReadings
{
  uint16_t mA;
  uint16_t mV;
  uint16_t mW;
};

extern Adafruit_INA219 ina219;
extern TaskHandle_t CurrentMonitorTaskHandle;
bool currentMonitorInit();
CurrentMonitorReadings currentMonitorRead();
