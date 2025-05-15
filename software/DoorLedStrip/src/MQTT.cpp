#include "MQTT.h"
#include "Settings.h"

#include <esp_wifi.h>
#include <WiFi.h>

bool _mqttConnected = false; // TODO: move this to separate class
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

char *mqttClientId()
{

  char *clientId = new char[24];
  sprintf(clientId, "DS-DLS-%s", WiFi.macAddress().c_str());
  return clientId;

  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK)
  {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
    snprintf(clientId, 24, "DS-DLS-%02x:%02x:%02x:%02x:%02x:%02x", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  }
  else
  {
    Serial.printf("Error getting MAC address: %d\n", ret);
    snprintf(clientId, 24, "DS-DLS-00:00:00:00:00:00");
  }
  return clientId;
}

void mqttInit()
{
  if (_settings.mqttCredentials.enabled)
  {
    Serial.println("[mqttInit] before setServer");
    Serial.printf("[mqttInit] server: %s port: %d client: %s user : %s password: %s \n", _settings.mqttCredentials.server, _settings.mqttCredentials.port, mqttClientId(), _settings.mqttCredentials.user, _settings.mqttCredentials.password);
    mqttClient.setServer(_settings.mqttCredentials.server, _settings.mqttCredentials.port);

    Serial.println("[mqttInit] after setServer");
    _mqttConnected = mqttClient.connect(mqttClientId(), _settings.mqttCredentials.user, _settings.mqttCredentials.password);

    Serial.printf("[mqttInit] connect returned status: %d\n", _mqttConnected);
    mqttClient.setKeepAlive(10); // FIXME: this doesn't seem to work
  }
  else
  {
    _mqttConnected = false;
  }
};

int lastMqttPublishCurrent = 0;
int lastMqttPublishLightSensor = 0;

bool mqttPublishCurrent(CurrentMonitorReadings cmr, bool force)
{
  if (!_mqttConnected)
  {
    return false;
  }

  if (!mqttClient.connected())
    mqttInit(); // FIXME: this hsouldn't be needed but there's some issue when publishing less frequent than every 15 seconds

  if (!force && (millis() - lastMqttPublishCurrent < 1000 * _settings.mqttCredentials.publishFrequency))
  {
    return true;
  }
  lastMqttPublishCurrent = millis();

  char *buffer = new char[256];
  sprintf(buffer, "%d", cmr.mA);
  bool ok1 = mqttClient.publish("dls/sensor/mA", buffer);
  sprintf(buffer, "%d", cmr.mV);
  bool ok2 = mqttClient.publish("dls/sensor/mV", buffer);
  Serial.printf("[mqttPublishCurrent] statuses: %d %d\n", ok1, ok2);
  return ok1 && ok2;
};
bool mqttPublishLightSensor(uint16_t lux, bool force)
{
  if (!_mqttConnected)
  {
    return false;
  }
  if (!mqttClient.connected())
    mqttInit(); // FIXME: this hsouldn't be needed but there's some issue when publishing less frequent than every 15 seconds

  if (!force && (millis() - lastMqttPublishLightSensor < 1000 * _settings.mqttCredentials.publishFrequency))
  {
    return true;
  }
  lastMqttPublishLightSensor = millis();

  char *buffer = new char[256];
  sprintf(buffer, "%d", lux);
  bool ok = mqttClient.publish("dls/sensor/lux", buffer);
  Serial.printf("[mqttPublishLightSensor] status: %d\n", ok);
  return ok;
};