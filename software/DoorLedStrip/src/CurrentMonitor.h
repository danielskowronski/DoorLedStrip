#pragma once
#include "Arduino.h"
#include "hardware.h"
#include <Adafruit_INA219.h>
#include "HomeSpan.h"

struct CurrentMonitorReadings
{
  int16_t mA;
  uint16_t mV;
  uint16_t mW;
};

extern Adafruit_INA219 ina219;
extern TaskHandle_t CurrentMonitorTaskHandle;
bool currentMonitorInit();
CurrentMonitorReadings currentMonitorRead();
uint16_t predictCurrent(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
double predictedOvercurrentRate(Pixel::Color a, Pixel::Color b);
