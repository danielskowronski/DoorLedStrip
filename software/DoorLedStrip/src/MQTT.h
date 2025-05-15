#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define MAX_STR_LEN 256
#include "CurrentMonitor.h"

extern bool _mqttConnected; // TODO: move this to separate class
struct MQTTcredentials
{
  bool enabled;
  uint32_t publishFrequency;
  char server[MAX_STR_LEN + 1];
  uint16_t port;
  char topic[MAX_STR_LEN + 1];
  char user[MAX_STR_LEN + 1];
  char password[MAX_STR_LEN + 1];
};

extern WiFiClient wifiClient;
extern PubSubClient mqttClient;

char *mqttClientId();
void mqttInit();
bool mqttPublishCurrent(CurrentMonitorReadings cmr, bool force = false);
bool mqttPublishLightSensor(uint16_t lux, bool force = false);
// bool mqttPublishActiveLight(); // TODO