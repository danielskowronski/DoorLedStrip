#include "State.h"
#include "LightMeter.h"
Pixel::Color colorRGB;
Pixel::Color colorWC;

State getState()
{
  State state;
  state.led.r = colorRGB.col[0];
  state.led.g = colorRGB.col[1];
  state.led.b = colorRGB.col[2];
  state.led.w = colorWC.col[3];
  state.AmbientLight = lightMeter.readLightLevel();
  state.night = _settings.nightLight;
  state.current = currentMonitorRead();
  return (state);
};
