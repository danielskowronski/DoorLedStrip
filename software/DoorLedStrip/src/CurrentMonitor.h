#pragma once
#include "Arduino.h"
#include "hardware.h"
#include <Adafruit_INA219.h>

extern Adafruit_INA219 ina219;
extern TaskHandle_t CurrentMonitorTaskHandle;
bool currentMonitorInit();