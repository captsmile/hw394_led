#pragma once

struct WeatherData {
  float temp;
  char  condSym;     // зараз
  float temp12h;
  char  condSym12h;  // через ~12 годин
  bool  valid;
};

extern WeatherData weather;

void fetchWeather();
void fmtWeatherLine(char* buf, int len, float temp, char sym);
