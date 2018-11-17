#include <avr/eeprom.h>

void save_low(char low) {
  eeprom_update_byte ((void *) 0, low);
}

void save_high(char high) {
  eeprom_update_byte ((void *) 1, high);
}

char get_high() {
  return eeprom_read_byte ((void *) 1);
}

char get_low() {
  return eeprom_read_byte ((void *) 0);
}