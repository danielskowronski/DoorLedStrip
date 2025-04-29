#include "OvercurrentWatchdog.h"
#include "Pixel.h"
#include "HomeSpan.h"
#include "HomeSpanLightBulb.h"
TaskHandle_t OvercurrentWatchdogTaskHandle;
void OvercurrentCheck()
{

  CurrentMonitorReadings readings = currentMonitorRead();
  Serial.printf("[OvercurrentWatchdogTask] Current: %d mA Voltage: %d mV Power: %d mW\n", readings.mA, readings.mV, readings.mW);
  if (readings.mA > OVERCURRENT_THRESHOLD || (readings.mV < UNDERVOLTAGE_THRESHOLD && readings.mV > 0))
  {

    Serial.printf("[OvercurrentWatchdogTask] Overcurrent or undervolatage detected! Current: %d mA Voltage: %d mV\n", readings.mA, readings.mV);
    homeSpanPAUSE;
    BulbRGB->power.setVal(0);
    BulbRGB->update();
    BulbW->power.setVal(0);
    BulbW->update();
    homeSpanRESUME;
    delay(50);
    pixel.set(Pixel::RGB(255, 0, 0, 0), 1);
    pixel.set(Pixel::RGB(255, 0, 0, 0), 1);
  }
}
void OvercurrentWatchdogTask(void *parameter)
{
  for (;;)
  {
    OvercurrentCheck();
    vTaskDelay(1000);
  }
}
