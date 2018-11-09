/********************************************
 *
 *  Name: Haoda Wang
 *  Email: haodawan@usc.edu
 *  Section: 12:30 PM Lab
 *  Assignment: Lab 7 - Rotary Encoder
 *
 ********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "lcd.h"

void variable_delay_us(int);
void play_note(unsigned short);

// Frequencies for natural notes from middle C (C4)
// up one octave to C5.
unsigned short frequency[8] =
{ 262, 294, 330, 349, 392, 440, 494, 523 };

unsigned char changed = 0;  // Flag for state change
unsigned char new_state, old_state;
int count = 0;		// Count to display
unsigned char a, b;

int main(void) {
	// Initialize DDR and PORT registers and LCD
	DDRC &= 0x00;
	PORTC |= (1 << PC5) | (1 << PC1);
	DDRB |= (1 << PB4);

	sei();
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT9) | (1 << PCINT13);

	lcd_init();
	lcd_writecommand(0x01);

	// Write a spash screen to the LCD
	lcd_moveto(0,3);
	lcd_stringout("Haoda Wang");
	lcd_moveto(1,5);
	lcd_stringout("Lab 7");
	_delay_ms(1000);
	lcd_writecommand(0x01);
	lcd_moveto(0,0);
	lcd_stringout("EE109 Lab 7 Poll");

	// Read the A and B inputs to determine the intial state
	// Warning: Do NOT read A and B separately.  You should read BOTH inputs
	// at the same time, then determine the A and B values from that value.
	unsigned char input = PINC;
	a = input & (1 << PC1);
	b = input & (1 << PC5);
	if (a && b)
		old_state = 3;
	else if (!a && !b)
		old_state = 0;
	else if (!a && b)
		old_state = 2;
	else
		old_state = 1;

	new_state = old_state;

	while (1) {                 // Loop forever
		char temp[12];
		sprintf(temp, "%d", changed);
		if (changed) { // Did state change?
			changed = 0;        // Reset changed flag
			// Clear lower line of LCD
			lcd_moveto(1,0);
			lcd_stringout("                ");
			// Output count to LCD
			lcd_moveto(1,0);
			char temp[12];
			sprintf(temp, "%d", count);
			lcd_stringout(temp);

			// Do we play a note?
			if ((count % 8) == 0) {
				// Determine which note (0-7) to play
				int note = (abs(count) % 64) / 8;
				// Find the frequency of the note
				unsigned short freq = frequency[note];
				// Call play_note and pass it the frequency
				play_note(freq);
			}
		}
	}
}

/*
  Play a tone at the frequency specified for one second
*/
void play_note(unsigned short freq)
{
	unsigned long period;

	period = 1000000 / freq;      // Period of note in microseconds

	while (freq--) {
		// Make PB4 high
		PORTB |= (1 << PB4);
		// Use variable_delay_us to delay for half the period
		variable_delay_us(period / 2);
		// Make PB4 low
		PORTB &= ~(1 << PB4);
		// Delay for half the period again
		variable_delay_us(period / 2);
	}
}

/*
	variable_delay_us - Delay a variable number of microseconds
*/
void variable_delay_us(int delay)
{
	int i = (delay + 5) / 10;

	while (i--)
		_delay_us(10);
}

ISR(PCINT1_vect) {
	// Read the input bits and determine A and B
	unsigned char input = PINC;
	a = input & (1 << PC1);
	b = input & (1 << PC5);

	// For each state, examine the two input bits to see if state
	// has changed, and if so set "new_state" to the new state,
	// and adjust the count value.
	if (old_state == 0) {
		// Handle A and B inputs for state 0
		if (a) {
			new_state = 1;
			count++;
		}
		else if (b) {
			new_state = 2;
			count--;
		}
	}
	else if (old_state == 1) {
		// Handle A and B inputs for state 1
		if (!a) {
			new_state = 0;
			count--;
		}
		else if (b) {
			new_state = 3;
			count++;
		}
	}
	else if (old_state == 2) {
		// Handle A and B inputs for state 2
		if (a) {
			new_state = 3;
			count--;
		}
		else if (!b) {
			new_state = 0;
			count++;
		}
	}
	else {   // old_state = 3
		// Handle A and B inputs for state 3
		if (!b) {
			new_state = 1;
			count--;
		}
		else if (!a) {
			new_state = 2;
			count++;
		}
	}

	// If state changed, update the value of old_state,
	// and set a flag that the state has changed.
	if (new_state != old_state) {
		changed = 1;
		old_state = new_state;
	}
}