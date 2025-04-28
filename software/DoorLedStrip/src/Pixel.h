#pragma once
#include "Arduino.h"
#include "HomeSpan.h"
#include "hardware.h"

extern Pixel pixel;
void rgbToHsv(uint8_t r, uint8_t g, uint8_t b, float &h, float &s, float &v);
