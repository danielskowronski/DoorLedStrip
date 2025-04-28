#include "CurrentMonitor.h"

Adafruit_INA219 ina219;
TaskHandle_t CurrentMonitorTaskHandle;
bool currentMonitorInit(){
  bool initStatus = ina219.begin();
  return initStatus;
}
