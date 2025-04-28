#include "CurrentMonitor.h"

Adafruit_INA219 ina219;
TaskHandle_t CurrentMonitorTaskHandle;
bool currentMonitorInit(){
  bool initStatus = ina219.begin();
  ina219.setCalibration_32V_2A();
  return initStatus;
}
CurrentMonitorReadings currentMonitorRead(){
  CurrentMonitorReadings readings;
  readings.mA = int(1000*ina219.getCurrent_mA());
  readings.mV = int(1000*ina219.getBusVoltage_V());
  readings.mW = int(ina219.getPower_mW());
  return readings;
}