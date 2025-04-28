#include <Preferences.h>
#include "Settings.h"

Preferences _flashPreferences;
Settings _settings;

void initSettings()
{
  _settings.magicNumber = MAGIC_NUMBER;
  _settings.nightLight.thresholdOn = DEFAULT_LUX_THRESHOLD_ON;
  _settings.nightLight.thresholdOff = DEFAULT_LUX_THRESHOLD_OFF;
  _settings.nightLight.targetBrightness = DEFAULT_TARGET_BRIGHTNESS;
  savePreferences();
}
void loadPreferences()
{
  _flashPreferences.begin("settings", true);
  size_t size = _flashPreferences.getBytesLength("settings");
  if (size == sizeof(_flashPreferences))
  {
    _flashPreferences.getBytes("settings", &_flashPreferences, sizeof(_flashPreferences));
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
