/********************************************
 *
 *  Name: Haoda Wang
 *  Email: haodawan@usc.edu
 *  Section: 12:30F
 *  Assignment: Lab 10 - Thermostat
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "lcd.h"
#include "adc.h"

int main(void) {
  // Set up ports
  
  // Set up interrupts
  sei();
  // Initialize the LCD
  lcd_init();
  lcd_writecommand(1);
  // Set up LCD
  lcd_stringout("Temp:    Rmt:");
  lcd_moveto(1, 0);
  lcd_stringout("Low=    High=");

}