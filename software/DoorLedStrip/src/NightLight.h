#pragma once
#include "Arduino.h"
#include "Settings.h"
#include "State.h"

extern TaskHandle_t NightLightTaskHandle;
bool areLightsOff();
void NightLightTask(void *parameter);
