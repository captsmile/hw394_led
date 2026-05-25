#include <EEPROM.h>
#include "storage.h"

Config cfg;

void initStorage() {
  EEPROM.begin(sizeof(Config));
}

bool loadConfig() {
  EEPROM.get(0, cfg);
  return cfg.magic == MAGIC_VAL;
}

void saveConfig() {
  cfg.magic = MAGIC_VAL;
  EEPROM.put(0, cfg);
  EEPROM.commit();
}

