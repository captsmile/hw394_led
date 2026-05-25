#pragma once
#include <pgmspace.h>

const char SETUP_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html><head>
  <meta charset=UTF-8>
  <meta name=viewport content="width=device-width,initial-scale=1">
  <title>Clock Setup</title>
  <style>
    body { font-family: sans-serif; max-width: 400px; margin: 30px auto; padding: 15px; color: #2c3e50; }
    h2   { margin-bottom: 20px; }
    label { display: block; margin: 12px 0 3px; font-size: 14px; font-weight: bold; }
    input { width: 100%; padding: 8px; box-sizing: border-box; border: 1px solid #bdc3c7; border-radius: 4px; }
    small { color: #95a5a6; font-size: 12px; }
    button { width: 100%; padding: 12px; background: #27ae60; color: #fff; border: none;
             margin-top: 20px; cursor: pointer; font-size: 16px; border-radius: 4px; }
    button:hover { background: #219a52; }
  </style>
</head><body>
<h2>&#9200; Clock Setup</h2>
<form method=POST action=/save>
  <label>WiFi SSID
    <input name=ssid maxlength=32 required>
  </label>
  <label>Password
    <input name=pass type=password maxlength=64>
  </label>
  <label>Timezone (POSIX)
    <input name=tz value="EET-2EEST,M3.5.0/3,M10.5.0/4" maxlength=64 required>
    <small>Ukraine: EET-2EEST,M3.5.0/3,M10.5.0/4</small>
  </label>
  <label>City (for weather)
    <input name=city placeholder="Kyiv" maxlength=32 required>
  </label>
  <label>OpenWeatherMap API Key
    <input name=apikey placeholder="paste your key here" maxlength=32>
    <small>Free key: openweathermap.org/api</small>
  </label>
  <button>Save &amp; Restart</button>
</form>
</body></html>
)rawliteral";
