#pragma once
#include "Arduino.h"
#include "HomeSpan.h"
#include "hardware.h"
#include "NightLight.h"
#include "State.h"
#include "CurrentMonitor.h"

struct LEDState {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t w;
};
struct State {
  LEDState led;
  NightLightSettings night;
  CurrentMonitorReadings current;
};

extern Pixel::Color colorRGB; 
extern Pixel::Color colorWC;

State getState();