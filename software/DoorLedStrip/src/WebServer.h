#pragma once
#include "Arduino.h"
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
extern AsyncWebServer server;
void setupWebServer();