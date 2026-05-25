#include <ESP8266WiFi.h>
#include "web_portal.h"
#include "config.h"
#include "storage.h"
#include "display.h"
#include "html.h"

ESP8266WebServer server(80);
bool apMode = false;

static void handleRoot() {
  server.send_P(200, "text/html", SETUP_HTML);
}

static void handleSave() {
  if (!server.hasArg("ssid") || !server.hasArg("tz") || !server.hasArg("city")) {
    server.send(400, "text/plain", "Missing required fields");
    return;
  }
  strncpy(cfg.ssid,     server.arg("ssid").c_str(),    32); cfg.ssid[32]     = 0;
  strncpy(cfg.password, server.arg("pass").c_str(),    64); cfg.password[64] = 0;
  strncpy(cfg.tz,       server.arg("tz").c_str(),      64); cfg.tz[64]       = 0;
  strncpy(cfg.city,     server.arg("city").c_str(),    32); cfg.city[32]     = 0;
  strncpy(cfg.api_key,  server.arg("apikey").c_str(),  32); cfg.api_key[32]  = 0;
  saveConfig();
  server.send(200, "text/html", "<h2>Saved! Restarting...</h2>");
  delay(2000);
  ESP.restart();
}

void startAP() {
  apMode = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ClockSetup", "12345678");
  server.on("/",     HTTP_GET,  handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();
  showZones("SETUP", "AP");
  Serial.println("AP: ClockSetup / 12345678  ->  192.168.4.1");
}

bool connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(cfg.ssid, cfg.password);
  showZones("WIFI", "....");
  for (int i = 0; i < WIFI_CONNECT_TRIES && WiFi.status() != WL_CONNECTED; i++) {
    delay(500);
  }
  return WiFi.status() == WL_CONNECTED;
}
