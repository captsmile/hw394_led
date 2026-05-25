#pragma once
#include <ESP8266WebServer.h>

extern ESP8266WebServer server;
extern bool apMode;

void startAP();
bool connectWiFi();
void startConfigServer();
