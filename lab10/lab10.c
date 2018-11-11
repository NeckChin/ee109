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

int temp = 72;

int main(void) {
  // Set up ports
  DDRB |= (1 << PB4);
  DDRD |= (1 << PD2) | (1 << PD3);
  PORTB |= (1 << PB3) | (1 << PB4);
  PORTC |= (1 << PC5) | (1 << PC1);
  // Set up interrupts
  sei();
  PCICR |= (1 << PCIE0) | (1 << PCIE1);
  PCMSK0 |= (1 << PCINT3) | (1 << PCINT4);
  PCMSK1 |= (1 << PCINT9) | (1 << PCINT13);
  // Initialize the LCD
  lcd_init();
  lcd_writecommand(1);
  // Initialize encoder
  encoder_init();
  // Set up LCD
  lcd_stringout("Temp:   Rmt:");
  lcd_moveto(1, 0);
  char buf[16];
  snprintf(buf, 16, "Low=%2d High=%2d  ", get_low_temp(), get_high_temp());
  lcd_stringout(buf);

  while(1) {
    if(temp < get_low_temp()) {
      PORTD &= ~(1 << PD2);
      PORTD |= (1 << PD3);
    }
    else if(temp > get_high_temp()) {
      PORTD &= ~(1 << PD3);
      PORTD |= (1 << PD2);
    }
    else {
      PORTD &= ~((1 << PD3) | (1 << PD2));
    }

    if(temp_has_change()) {
      lcd_moveto(1, 0);
      snprintf(buf, 16, "Low=%2d High=%2d  ", get_low_temp(), get_high_temp());
      lcd_stringout(buf);
    }
  }
}