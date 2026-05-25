#pragma once
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include "custom_font.h"

extern MD_Parola display;

void initDisplay();
void showZones(const char* top, const char* bot);
