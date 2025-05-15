#include "Settings.h"

Preferences _flashPreferences;
Settings _settings;

void initSettings()
{
  _settings.magicNumber = MAGIC_NUMBER;

  _settings.nightLight.thresholdOn = DEFAULT_LUX_THRESHOLD_ON;
  _settings.nightLight.thresholdOff = DEFAULT_LUX_THRESHOLD_OFF;
  _settings.nightLight.targetBrightness = DEFAULT_TARGET_BRIGHTNESS;

  _settings.mqttCredentials.enabled = false;
  _settings.mqttCredentials.publishFrequency = 30;
  strncpy(_settings.mqttCredentials.server, "server", MAX_STR_LEN);
  _settings.mqttCredentials.port = 1883;
  strncpy(_settings.mqttCredentials.topic, "DoorLedStrip", MAX_STR_LEN);
  strncpy(_settings.mqttCredentials.user, "user", MAX_STR_LEN);
  strncpy(_settings.mqttCredentials.password, "password", MAX_STR_LEN);

  savePreferences();
}
void loadPreferences()
{
  _flashPreferences.begin("settings", true);
  size_t size = _flashPreferences.getBytesLength("settings");
  if (size == sizeof(_settings))
  {
    _flashPreferences.getBytes("settings", &_settings, sizeof(_settings));
    _flashPreferences.end();
  }
  else
  {
    initSettings();
  }
  if (_settings.magicNumber != MAGIC_NUMBER)
  {
    initSettings();
  }
}
void savePreferences()
{
  _flashPreferences.begin("settings", false);
  _flashPreferences.putBytes("settings", &_settings, sizeof(_settings));
  _flashPreferences.end();
}
