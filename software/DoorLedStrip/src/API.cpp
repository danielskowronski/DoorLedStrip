#include "Arduino.h"
#include "API.h"
#include "hardware.h"

#include "State.h"
#include "Pixel.h"
#include "HomeSpanLightBulb.h"
#include <ArduinoJson.h>

AsyncWebServer *API_SERVER = nullptr;

String stateToJSON(const State &state)
{
  StaticJsonDocument<256> doc;
  doc["led"]["r"] = state.led.r;
  doc["led"]["g"] = state.led.g;
  doc["led"]["b"] = state.led.b;
  doc["led"]["w"] = state.led.w;
  doc["light"] = state.AmbientLight;
  doc["night"]["thresholdOn"] = state.night.thresholdOn;
  doc["night"]["thresholdOff"] = state.night.thresholdOff;
  doc["night"]["targetBrightness"] = state.night.targetBrightness;
  doc["current"]["mA"] = state.current.mA;
  doc["current"]["mV"] = state.current.mV;
  doc["current"]["mW"] = state.current.mW;

  String json;
  serializeJson(doc, json);
  return json;
}
void setupWebServer()
{
  API_SERVER = new AsyncWebServer(PORT_API);
  if (!LittleFS.begin(true, "/littlefs", 10U, "littlefs"))
  {
    Serial.println("Failed to mount LittleFS");
    return;
  }
  else
  {
    Serial.println("LittleFS mounted successfully");
  }

  API_SERVER->serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  API_SERVER->on("/api/state.json", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
    String json = stateToJSON(getState());
    request->send(200, "application/json", json); });

  API_SERVER->on("/api/setColor", HTTP_POST, [](AsyncWebServerRequest *request)
                 {
    int r = request->arg("r").toInt();
    int g = request->arg("g").toInt();
    int b = request->arg("b").toInt();
    int w = request->arg("w").toInt();

    if (r<0||r>255 || g<0||g>255 || b<0||b>255 || w<0||w>255) {
      request->send(400, "text/plain", "Invalid RGBW values");
      return;
    }
  
    homeSpanPAUSE;
    colorRGB = Pixel::RGB(r, g, b, 0);
    colorWC = Pixel::RGB(0, 0, 0, w);
    float h, s, v;
    rgbToHsv(r, g, b, h, s, v);
    
    if (v > 0) {
      BulbRGB->H.setVal(h);
      BulbRGB->S.setVal(s*100);
      BulbRGB->V.setVal(v * 100);
      BulbRGB->power.setVal(1);
    }
    else {
      BulbRGB->power.setVal(0);
    }
    BulbRGB->update();

    int wv=int(w/2.55);

    if (wv > 0) {
      BulbW->V.setVal(w/2.55);
      BulbW->power.setVal(1);
    } else {
      BulbW->power.setVal(0);
    }
    BulbW->update();
    
    homeSpanRESUME;

    Serial.printf("Color requested: R=%3d G=%3d B=%3d W=%3d -> RGB: H=%3d S=%3d V=%3d + W: V=%3d\n", r, g, b, w, int(h), int(s), int(v), wv);
  
    request->redirect("/"); });

  API_SERVER->on("/api/setNight", HTTP_POST, [](AsyncWebServerRequest *request)
                 {
    int thresholdOn = request->arg("thresholdOn").toInt();
    int thresholdOff = request->arg("thresholdOff").toInt();
    int targetBrightness = request->arg("targetBrightness").toInt();

    if (!(thresholdOn>=0&&thresholdOn<=65535 && thresholdOff>=0&&thresholdOff<=65535 && targetBrightness>=0&&targetBrightness<=255)) {
      Serial.println("Invalid night settings");
      request->send(400, "text/plain", "Invalid night settings");
      return;
    }

    _settings.nightLight.thresholdOn = thresholdOn;
    _settings.nightLight.thresholdOff = thresholdOff;
    _settings.nightLight.targetBrightness = targetBrightness;
    savePreferences();
  
    request->redirect("/settings.html"); });

  API_SERVER->begin();
  Serial.println("HTTP server started");
}
