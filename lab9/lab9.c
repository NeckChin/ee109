/********************************************
 *
 *  Name: Haoda Wang
 *  Email: haodawan@usc.edu
 *  Section: 12:30PM Friday
 *  Assignment: Lab 9 - Serial Communications
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "adc.h"

// Serial communications functions and variables
void serial_init(unsigned short);
void serial_stringout(char *);
void serial_txchar(char);

#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0 register

// ADC functions and variables
void adc_init(unsigned char);

#define ADC_CHAN 0              // Buttons use ADC channel 0

char *messages[] = {
    "Hello           ",
    "How are you?    ",
    "owo what's this ",
    "uwu             ",
    "Redekopp-senpai?",
    "To be fair, you ",
    "need a high IQ  ",
    "to understand   ",
    "Rick and Morty  ",
    "nicememe.jpeg   "
};

short index = 0;
short char_nums = 0;
char finished = 0;
char buffer[17];

int main(void) {
    // Initialize the modules and LCD
    lcd_init();
    lcd_writecommand(0x01);
    adc_init(ADC_CHAN);
    serial_init(MYUBRR);

    UCSR0B |= (1 << RXCIE0);    // Enable receiver interrupts
    sei();                      // Enable interrupts

    // Show the splash screen
    lcd_moveto(0,3);
    lcd_stringout("Haoda Wang");
    lcd_moveto(1,5);
    lcd_stringout("Lab 9");
    _delay_ms(1000);
    lcd_writecommand(0x01);

    // Show initial message
    lcd_moveto(0, 0);
    lcd_stringout(messages[index]);

    while (1) {                 // Loop forever
        // Get an ADC sample
        unsigned char adc_result = adc_sample();

        // Up button pressed?
        if(abs(51 - adc_result) < 5) {
            index = index == 9 ? 0 : index + 1;
            lcd_moveto(0, 0);
            lcd_stringout(messages[index]);
            _delay_ms(200);
        }

        // Down button pressed?
        if(abs(101 - adc_result) < 5) {
            index = index == 0 ? 9 : index - 1;
            lcd_moveto(0, 0);
            lcd_stringout(messages[index]);
            _delay_ms(200);
        }

        // Select button pressed?
        if(abs(205 - adc_result) < 5) {
            serial_stringout(messages[index]);
            _delay_ms(200);
        }

        // Message received from remote device?
        if(finished) {
            finished = 0;
            lcd_moveto(1, 0);
            lcd_stringout(buffer);
        }
    }
}

/* ----------------------------------------------------------------------- */

void serial_init(unsigned short ubrr_value)
{
    UBRR0 = ubrr_value;
    // Set up USART0 registers
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);
    UCSR0C = (3 << UCSZ00);
    // Enable tri-state
    DDRD |= (1 << PD3);
    PORTD &= ~(1 << PD3);
}

void serial_txchar(char ch)
{
    while ((UCSR0A & (1<<UDRE0)) == 0);
    UDR0 = ch;
}

void serial_stringout(char *s)
{
    // Call serial_txchar in loop to send a string
    for(int i = 0; i < 16; i++) {
        serial_txchar(s[i]);
    }
}

ISR(USART_RX_vect)
{
    char ch;
    // Handle received character
    ch = UDR0;
    // Store in buffer
    buffer[char_nums] = ch;
    // If message complete, set flag
    char_nums++;
    if(char_nums == 16) {
        buffer[16] = '\0';
        finished = 1;
        char_nums = 0;
    }
}
