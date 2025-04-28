#pragma once
#include "Arduino.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
extern AsyncWebServer *API_SERVER;
void setupWebServer();
