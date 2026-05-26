#pragma once
#include <Arduino.h>

// --- Display pins ---
#define DATA_PIN    2
#define CS_PIN      0
#define CLK_PIN     4
#define MAX_DEVICES 8
#define ZONE_BOTTOM 0
#define ZONE_TOP    1

// --- Timings (ms) ---
#define WIFI_CONNECT_TRIES  20
#define WEATHER_UPDATE_MS    600000UL   // 10 хвилин

// --- NTP ---
#define NTP_SERVER "pool.ntp.org"

// --- EEPROM ---
#define MAGIC_VAL 0xAB

struct __attribute__((packed)) Config {
  uint8_t magic;
  char ssid[33];
  char password[65];
  char tz[65];      // POSIX timezone string
  char city[33];    // city for weather
  char api_key[33]; // OpenWeatherMap API key
};

extern Config cfg;
