#pragma once
#include "Arduino.h"
#include "hardware.h"
#include "CurrentMonitor.h"

extern TaskHandle_t OvercurrentWatchdogTaskHandle;
void OvercurrentWatchdogTask(void *parameter);
void OvercurrentCheck();
