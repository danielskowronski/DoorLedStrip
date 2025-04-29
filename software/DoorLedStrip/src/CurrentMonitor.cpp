#include "CurrentMonitor.h"

Adafruit_INA219 ina219;
TaskHandle_t CurrentMonitorTaskHandle;
bool currentMonitorInit()
{
  bool initStatus = ina219.begin();
  Serial.printf("[currentMonitorInit] ina219.begin() status: %d\n", initStatus);
  ina219.setCalibration_32V_2A();
  return initStatus;
}
CurrentMonitorReadings currentMonitorRead()
{
  CurrentMonitorReadings readings;
  readings.mA = int(ina219.getCurrent_mA());
  readings.mV = int(1000 * ina219.getBusVoltage_V() + ina219.getShuntVoltage_mV());
  readings.mW = int(ina219.getPower_mW());
  return readings;
}

uint16_t predictCurrent(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  // RGB: max value 255 =>  500mA
  //   W: max value 255 => 1000mA
  return r * 2 + g * 2 + b * 2 + w * 4;
}
double predictedOvercurrentRate(Pixel::Color a, Pixel::Color b)
{
  uint16_t predictedCurrent = predictCurrent(a.col[0] + b.col[0], a.col[1] + b.col[1], a.col[2] + b.col[2], a.col[3] + b.col[3]);
  double rate = double(predictedCurrent) / OVERCURRENT_THRESHOLD;
  return rate;
}