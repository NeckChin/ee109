/********************************************
 *
 *  Name: Haoda Wang
 *  Email: haodawan@usc.edu
 *  Section: 12:30F
 *  Assignment: Lab 10 - Thermostat
 *
 ********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "highlow.h"
#include "serial.h"
#include "ds18b20.h"
#include "eeprom.h"

unsigned short temp = 0;
short previous = 72;
short remote = 72;

int main(void) {
  // Set up ports
  DDRB |= (1 << PB4);
  DDRC |= (1 << PC2) | (1 << PC3);
  PORTB |= (1 << PB3) | (1 << PB4);
  PORTC |= (1 << PC4) | (1 << PC1);
  // Set up interrupts
  sei();
  PCICR |= (1 << PCIE0) | (1 << PCIE1);
  PCMSK0 |= (1 << PCINT3) | (1 << PCINT4);
  PCMSK1 |= (1 << PCINT9) | (1 << PCINT12);
  UCSR0B |= (1 << RXCIE0);
  lcd_init();           // Initialize the LCD
  encoder_init();       // Initialize encoder
  serial_init();        // Set up serial communication
  ds_init();            // Reset DS18B20

  // Get temp thresholds
  if(get_low() <= 100 && get_low() >= 40)
    low_temp = get_low();
  if(get_high() <= 100 && get_high() >= 40)
    high_temp = get_high();

  // Useful temp variables
  char buf[17];
  unsigned char cel[2];

  // Reset LCD
  lcd_moveto(1, 0);
  lcd_writecommand(1);
  snprintf(buf, 17, "Low=%3d High=%3d", low_temp, high_temp);
  lcd_moveto(1, 0);
  lcd_stringout(buf);

  while(1) {
    previous = temp;
    ds_temp(cel);
    temp = ((9 * ((cel[1] << 8) | cel[0])) + 2560) / 80;

    if(temp < low_temp) {
      PORTC &= ~(1 << PC2);
      PORTC |= (1 << PC3);
    }
    else if(temp > high_temp) {
      PORTC &= ~(1 << PC3);
      PORTC |= (1 << PC2);
    }
    else {
      PORTC &= ~((1 << PC3) | (1 << PC2));
    }

    if(temp_has_change()) {
      snprintf(buf, 17, "Low=%3d High=%3d", low_temp, high_temp);
      lcd_moveto(1, 0);
      lcd_stringout(buf);
      save_low(low_temp);
      save_high(high_temp);
    }
    if(valid_data() || previous != temp) {
      if(previous != temp)
        serial_tempout(temp);
      snprintf(buf, 17, "Temp:%3d Rmt:%3d", temp, remote_temp());
      lcd_moveto(0, 0);
      lcd_stringout(buf);
    }
  }
}