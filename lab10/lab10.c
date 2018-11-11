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

#include "encoder.h"
#include "adc.h"

unsigned char cold_hot = 0;
unsigned char toggle_changed = 0;
int heat_temp = 80;
int old_heat_temp = 80;
int cool_temp = 60;
int old_cool_temp = 80;

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
  // Set up LCD
  lcd_stringout("Temp:    Rmt:");
  lcd_moveto(1, 0);
  lcd_stringout("Low=    High=");
  lcd_writecommand(1);
  // Initialize encoder
  encoder_init();

  while(1) {
    if(toggle_changed) {
      reset_temp();
    }
    lcd_moveto(0, 0);
    char buf[16];
    snprintf(buf, 16, "%6d", get_temp());
    lcd_stringout(buf);
  }
}

ISR(PCINT0_vect) {
    if((PINB & (1 << PB3)) == 0) {
      if(cold_hot == 1) {
        cold_hot = 0;
        toggle_changed = 1;
      }
    }
    else if((PINB & (1 << PB4)) == 0) {
      if(cold_hot == 0) {
        cold_hot = 1;
        toggle_changed = 1;
      }
    }
}