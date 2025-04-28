#pragma once
#include "Arduino.h"
#include <Preferences.h>

#define MAGIC_NUMBER 0xDEADBEEF
#define DEFAULT_LUX_THRESHOLD 100
#define DEFAULT_TARGET_BRIGHTNESS 32

extern Preferences _flashPreferences;

struct NightLightSettings {
  uint16_t luxThreshold;
  uint8_t targetBrightness;
};
struct Settings {
  uint32_t magicNumber;
  NightLightSettings nightLight;
};
extern Settings _settings;

void initSettings();
void loadPreferences();
void savePreferences();
