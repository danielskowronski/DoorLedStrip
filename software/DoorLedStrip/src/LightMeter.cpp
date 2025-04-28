#include "LightMeter.h"
BH1750 lightMeter(LIGHTMETER_ADDRESS);
bool lightMeterInit()
{
  bool initStatus = lightMeter.begin();
  return initStatus;
}
