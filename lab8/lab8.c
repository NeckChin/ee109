/********************************************
 *
 *  Name: Haoda Wang
 *  Email: haodawan@usc.edu
 *  Section: 12:30 PM Lab
 *  Assignment: Lab 8 - Digital Stopwatch
 *
 ********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

#include "lcd.h"
#include "adc.h"

int state = 0; // State for state machine
char changed = 0;
short time[3] = {0, 0, 0}; // Array for timer

int main(void) {
	lcd_init();				// Initialize LCD
	lcd_writecommand(0x01);
	adc_init(0);			// Initialize ADC

    // Write splash screen
    lcd_moveto(0,3);
    lcd_stringout("Haoda Wang");
    lcd_moveto(1,5);
    lcd_stringout("Lab 8");
    _delay_ms(1000);
    lcd_writecommand(0x01);

	// Initialize Timer
	sei();					// Set interrupts
	TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS11);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 25000;

	// Print out empty time
	lcd_moveto(0, 0);
	lcd_writedata(time[0] + '0');
	lcd_moveto(0, 1);
	lcd_writedata(time[1] + '0');
	lcd_moveto(0, 2);
	lcd_writedata('.');
	lcd_moveto(0, 3);
	lcd_writedata(time[2] + '0');

	while(1) {
		// Check inputs
		unsigned char adc_result = adc_sample();
		if(abs(50 - adc_result) < 5) {
			if(state == 0)
				state = 1;
			else if(state == 1)
				state = 0;
			_delay_ms(100);
		}
		else if(abs(100 - adc_result) < 5) {
			if(state == 1)
				state = 2;
			else if(state == 2)
				state = 1;
			else if(state == 0) {
				// Reset time and print
				time[0] = 0;
				time[1] = 0;
				time[2] = 0;

				lcd_moveto(0, 0);
				lcd_writedata(time[0] + '0');
				lcd_moveto(0, 1);
				lcd_writedata(time[1] + '0');
				lcd_moveto(0, 3);
				lcd_writedata(time[2] + '0');
			}
			_delay_ms(100);
		}
		// Update time only if state is 1
		if(state == 1 && changed == 1) {
			lcd_moveto(0, 0);
			lcd_writedata(time[0] + '0');
			lcd_moveto(0, 1);
			lcd_writedata(time[1] + '0');
			lcd_moveto(0, 3);
			lcd_writedata(time[2] + '0');
			changed == 0;
		}
	}
}

ISR(TIMER1_COMPA_vect){
	// Only increment when state is 1 or 2
	if(state != 0) {
		time[2]++;
		// Reset to 0 and add on if over 10
		if(time[2] == 10) {
			time[2] = 0;
			time[1]++;
			// Reset to 0 and add on if over 10
			if(time[1] == 10) {
				time[1] = 0;
				time[0]++;
				// If timer is at 60.0, reset all to 0
				if(time[0] == 6) {
					time[0] = 0;
				}
			}
		}
	}
	changed = 1;
}