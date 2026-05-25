#include <SPI.h>
#include "display.h"
#include "config.h"

MD_Parola display(MD_MAX72XX::FC16_HW, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

static char zTop[9];
static char zBot[9];

void initDisplay() {
  display.begin(MAX_DEVICES);
  display.setIntensity(3);
  display.displayClear();
  display.setZone(ZONE_BOTTOM, 0, 3);
  display.setZone(ZONE_TOP,    4, 7);
  display.setFont(const_cast<MD_MAX72XX::fontType_t*>(weatherFont));
}

void showZones(const char* top, const char* bot) {
  strncpy(zTop, top, 8); zTop[8] = 0;
  strncpy(zBot, bot, 8); zBot[8] = 0;
  display.displayZoneText(ZONE_BOTTOM, zBot, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  display.displayZoneText(ZONE_TOP,    zTop, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  display.displayAnimate();
}
