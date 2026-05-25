#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <math.h>
#include <time.h>
#include <climits>
#include "weather.h"
#include "custom_font.h"
#include "config.h"

WeatherData weather = {0.0f, '~', 0.0f, '~', false};

static char toSym(const char* cond) {
  if (!cond)                               return CHAR_CLOUD;
  if (strcmp(cond, "Clear")        == 0)   return CHAR_SUN;
  if (strcmp(cond, "Sunny")        == 0)   return CHAR_SUN;
  if (strcmp(cond, "Clouds")       == 0)   return CHAR_CLOUD;
  if (strcmp(cond, "Cloudy")       == 0)   return CHAR_CLOUD;
  if (strcmp(cond, "Overcast")     == 0)   return CHAR_CLOUD;
  if (strcmp(cond, "Partly cloudy") == 0)  return CHAR_PARTLY_CLOUD;
  if (strcmp(cond, "Snow")         == 0)   return CHAR_SNOW;
  if (strcmp(cond, "Drizzle")      == 0)   return CHAR_RAIN;
  if (strcmp(cond, "Rain")         == 0)   return CHAR_RAIN;
  if (strcmp(cond, "Thunderstorm") == 0)   return CHAR_THUNDERSTORM;
  if (strcmp(cond, "Mist")         == 0)   return CHAR_FOG;
  if (strcmp(cond, "Fog")          == 0)   return CHAR_FOG;
  if (strcmp(cond, "Smoke")        == 0)   return CHAR_FOG;
  if (strcmp(cond, "Haze")         == 0)   return CHAR_FOG;
  return CHAR_CLOUD;  // default fallback
}

static void fetchCurrent(WiFiClient& client) {
  HTTPClient http;
  char url[200];
  snprintf(url, sizeof(url),
    "http://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric",
    cfg.city, cfg.api_key);

  if (!http.begin(client, url)) return;
  int code = http.GET();
  Serial.printf("Current HTTP %d\n", code);

  if (code == HTTP_CODE_OK) {
    JsonDocument filter;
    filter["main"]["temp"]       = true;
    filter["weather"][0]["main"] = true;

    JsonDocument doc;
    if (!deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter))) {
      weather.temp    = doc["main"]["temp"].as<float>();
      weather.condSym = toSym(doc["weather"][0]["main"].as<const char*>());
      weather.valid   = true;
    }
  }
  http.end();
}

static void fetchForecast12h(WiFiClient& client) {
  HTTPClient http;
  char url[200];
  snprintf(url, sizeof(url),
    "http://api.openweathermap.org/data/2.5/forecast?q=%s&appid=%s&units=metric&cnt=8",
    cfg.city, cfg.api_key);

  if (!http.begin(client, url)) return;
  int code = http.GET();
  Serial.printf("Forecast HTTP %d\n", code);

  if (code != HTTP_CODE_OK) { http.end(); return; }

  JsonDocument filter;
  filter["list"][0]["dt"]                  = true;
  filter["list"][0]["main"]["temp"]        = true;
  filter["list"][0]["weather"][0]["main"]  = true;

  JsonDocument doc;
  if (deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter))) {
    http.end(); return;
  }

  time_t target   = time(nullptr) + 12 * 3600;
  long   bestDiff = LONG_MAX;

  for (JsonObject item : doc["list"].as<JsonArray>()) {
    time_t dt   = item["dt"].as<long>();
    long   diff = abs((long)(dt - target));
    if (diff < bestDiff) {
      bestDiff           = diff;
      weather.temp12h    = item["main"]["temp"].as<float>();
      weather.condSym12h = toSym(item["weather"][0]["main"].as<const char*>());
    }
  }
  http.end();
}

void fetchWeather() {
  if (cfg.api_key[0] == 0 || cfg.city[0] == 0) return;
  WiFiClient client;
  fetchCurrent(client);
  fetchForecast12h(client);
}

// e.g. "18°☀"  де ° з системного шрифту (0x90), символ умови — кастомний гліф
void fmtWeatherLine(char* buf, int len, float temp, char sym) {
  snprintf(buf, len, "%d%c%c", (int)roundf(temp), CHAR_DEGREE, sym);
}
