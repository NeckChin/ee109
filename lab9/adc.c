#include <avr/io.h>

#include "adc.h"


void adc_init(unsigned char channel)
{
    // Initialize the ADC
    ADMUX |= (0x60 | channel);
    ADCSRA |= 0x87;   
}

unsigned char adc_sample()
{
    // Convert an analog input and return the 8-bit result
	ADCSRA |= 0x40;
	while(ADCSRA & 0x40) {
	}
	return ADCH;
}
