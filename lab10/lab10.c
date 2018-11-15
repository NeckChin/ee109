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

#include "highlow.h"
#include "serial.h"
#include "ds18b20.h"

short temp = 72;
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
  ds_init();           // Reset DS18B20

  // Useful temp variables
  char buf[17];
  char cel[2];

  // Show splashscreen
  lcd_writecommand(1);
  lcd_moveto(0, 2);
  lcd_stringout("EE109 Lab 10");
  lcd_moveto(1, 3);
  lcd_stringout("Haoda Wang");
  _delay_ms(500);
  lcd_writecommand(1);
  // Reset LCD
  lcd_moveto(1, 0);
  snprintf(buf, 16, "Low=%2d High=%2d", low_temp, high_temp);
  lcd_stringout(buf);

  while(1) {
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
    }
    if(valid_data()) {
      snprintf(buf, 17, "Temp:%3d Rmt:%3d", temp, remote_temp());
      lcd_moveto(0, 0);
      lcd_stringout(buf);
    }

    previous = temp;
    temp = (cel[1] << 8) | cel[0];
    ds_temp(cel);

    serial_tempout(72);
  }
}