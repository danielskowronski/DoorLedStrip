#include "WebServer.h"
AsyncWebServer server(80);

void setupWebServer() {
  if (!LittleFS.begin(true, "/littlefs",10U, "littlefs")) {
    Serial.println("Failed to mount LittleFS");
    return;
  }
  else {
    Serial.println("LittleFS mounted successfully");
  }

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server.on("/api/state", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"state\": \"ok\"}";
    request->send(200, "application/json", json);
  });

  server.begin();
  Serial.println("HTTP server started");
}