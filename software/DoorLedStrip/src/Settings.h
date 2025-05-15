#pragma once
#include "Arduino.h"
#include <Preferences.h>

#include "Settings.h"
#include "MQTT.h"

#define MAGIC_NUMBER 0xDEADBEEF
#define DEFAULT_LUX_THRESHOLD_ON 1
#define DEFAULT_LUX_THRESHOLD_OFF 2
#define DEFAULT_TARGET_BRIGHTNESS 3

extern Preferences _flashPreferences;

struct NightLightSettings
{
  uint16_t thresholdOn;
  uint16_t thresholdOff;
  uint8_t targetBrightness;
};

struct Settings
{
  uint32_t magicNumber;
  NightLightSettings nightLight;
  MQTTcredentials mqttCredentials;
};
extern Settings _settings;

void initSettings();
void loadPreferences();
void savePreferences();
