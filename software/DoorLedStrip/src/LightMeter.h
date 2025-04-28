#pragma once
#include "Arduino.h"
#include "hardware.h"

#include <Wire.h>
#include <BH1750.h>

extern BH1750 lightMeter;
bool lightMeterInit();
