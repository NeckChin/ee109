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

#include "lcd.h"
#include "adc.h"

char cold_hot = 0;

int main(void) {
  // Set up ports
  PORTB |= (1 << PB3) | (1 << PB4);
  // Set up interrupts
  sei();
  PCICR |= (1 << PCIE0) | (1 << PCIE1);
  PCMSK0 |= (1 << PCINT3) | (1 << PCINT4);
  PORTC |= (1 << PC5) | (1 << PC1);
  DDRB |= (1 << PB4);
  // Initialize the LCD
  lcd_init();
  lcd_writecommand(1);
  // Set up LCD
  lcd_stringout("Temp:    Rmt:");
  lcd_moveto(1, 0);
  lcd_stringout("Low=    High=");
  lcd_writecommand(1);


}

ISR(PCINT0_vect) {
    if((PINB & (1 << PB3)) == 0) {
      cold_hot = 0;
    }
    if((PINB & (1 << PB4)) == 0) {
      cold_hot = 1;
    }
}