#include <Arduino.h>
#include <time.h>
#include "config.h"
#include "storage.h"
#include "display.h"
#include "web_portal.h"
#include "weather.h"

static const char* months[] = {
  "JAN","FEB","MAR","APR","MAY","JUN",
  "JUL","AUG","SEP","OCT","NOV","DEC"
};

enum Screen { SCR_DATETIME = 0, SCR_WEATHER = 1 };
static const unsigned long SCR_DURATION[] = { 10000, 8000 };

void setup() {
  delay(500);
  Serial.begin(9600);
  initStorage();
  initDisplay();

  if (!loadConfig() || !connectWiFi()) {
    startAP();
    return;
  }

  configTime(0, 0, NTP_SERVER);
  setenv("TZ", cfg.tz, 1);
  tzset();
  fetchWeather();
}

void loop() {
  if (apMode) {
    server.handleClient();
    return;
  }

  static unsigned long lastNtpSync = 0;
  static unsigned long lastWeather = millis();
  static unsigned long screenStart = 0;
  static Screen        screen      = SCR_DATETIME;

  unsigned long now_ms = millis();

  if (now_ms - lastNtpSync > NTP_RESYNC_MS) {
    configTime(0, 0, NTP_SERVER);
    setenv("TZ", cfg.tz, 1);
    tzset();
    lastNtpSync = now_ms;
  }

  if (now_ms - lastWeather > WEATHER_UPDATE_MS) {
    fetchWeather();
    lastWeather = now_ms;
  }

  if (now_ms - screenStart > SCR_DURATION[screen]) {
    screen = (Screen)(1 - screen);  // toggle 0 <-> 1
    screenStart = now_ms;
  }

  time_t ts = time(nullptr);
  struct tm* t = localtime(&ts);
  if (t->tm_year <= 100) return;

  char top[9], bot[9];

  switch (screen) {
    case SCR_DATETIME:
      snprintf(top, sizeof(top), "%02d %s", t->tm_mday, months[t->tm_mon]);
      snprintf(bot, sizeof(bot), "%02d:%02d", t->tm_hour, t->tm_min);
      break;

    case SCR_WEATHER:
      if (weather.valid) {
        fmtWeatherLine(top, sizeof(top), weather.temp,    weather.condSym);
        fmtWeatherLine(bot, sizeof(bot), weather.temp12h, weather.condSym12h);
      } else {
        snprintf(top, sizeof(top), "NO WX");
        snprintf(bot, sizeof(bot), "....");
      }
      break;
  }

  showZones(top, bot);
  delay(1000);
}
