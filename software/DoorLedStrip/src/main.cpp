#include <Arduino.h>

#define LED_COUNT 60
#include <NeoPixelBus.h>
NeoPixelBus<NeoGrbwFeature, NeoEsp32I2s1X8Sk6812Method> led(LED_COUNT, 16); 

#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;
BH1750 lightMeter(0x23);

void print_sensors(){
  Serial.printf("%2.2f V | %4d mA | %5d lx\n", ina219.getBusVoltage_V(), int(ina219.getCurrent_mA()), int(lightMeter.readLightLevel()));
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Initializing...");
  Serial.flush();

  Wire.begin();

  lightMeter.begin();
  ina219.begin();
  led.Begin();

  Serial.println();
  Serial.println("Running...");

  Serial.println("Setting all RED");
  for (int i=0; i<LED_COUNT; i++){
    led.SetPixelColor(i, RgbwColor(255, 0, 0, 0));
    led.Show();
    delay(10);
  }
  print_sensors();
  Serial.println("Setting all GREEN");
  for (int i=0; i<LED_COUNT; i++){
    led.SetPixelColor(i, RgbwColor(0, 255, 0, 0));
    led.Show();
    delay(10);
  }
  print_sensors();
  Serial.println("Setting all BLUE");
  for (int i=0; i<LED_COUNT; i++){
    led.SetPixelColor(i, RgbwColor(0, 0, 255, 0));
    led.Show();
    delay(10);
  }
  print_sensors();
  Serial.println("Setting all WHITE");
  for (int i=0; i<LED_COUNT; i++){
    led.SetPixelColor(i, RgbwColor(0, 0, 0, 255));
    led.Show();
    delay(10);
  }
  print_sensors();

  Serial.println("In 3s, setting all RGB to RANDOM every 1s");
  delay(3000);
}

void loop() {
  int r=random(256);
  int g=random(256);
  int b=random(256);
  for (int i=0; i<LED_COUNT; i++){
    led.SetPixelColor(i, RgbwColor(r,g,b, 0));
  }
  led.Show();
  print_sensors();
  delay(1000);
}