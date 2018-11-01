/********************************************
 *
 *  Name: Haoda Wang
 *  Email: haodawan@usc.edu
 *  Section: 12:30F
 *  Assignment: Lab 6 - Analog-to-digital conversion
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "lcd.h"
#include "adc.h"

// Clear the LCD
void clear_lcd() {
    int space = 32;
    lcd_moveto(0, 0);
    for (int i = 0; i < 16; i++) lcd_stringout(&space);
    lcd_moveto(1, 0);
    for (int i = 0; i < 16; i++) lcd_stringout(&space);
}

int main(void)
{
    unsigned char adc_result;

    // Initialize the LCD
    lcd_init();
    clear_lcd();

    // Initialize the ADC
    adc_init(3);

    // Use the ADC to find a seed for rand();
    adc_result = adc_sample();
    srand(adc_result << 8 | adc_result);  // Make a 16-bit number for srand()

    // Write splash screen
    lcd_moveto(0,3);
    lcd_stringout("Haoda Wang");
    lcd_moveto(1,5);
    lcd_stringout("Lab 6");
    _delay_ms(1000);
    clear_lcd();

    // Find a random initial position of the 'X'
    short loc = rand() % 16;

    // Display the 'X' on the screen
    lcd_moveto(0, loc);
    lcd_stringout("X");

    int count = 0;
    int caret = 0;
    int old_caret = caret;

    while(1) {                 // Loop forever
        // Do a conversion
        caret = adc_sample() / 16;

        // Move '^' to new position
        if(caret != old_caret) {
            // Clear old caret location
            lcd_moveto(0, old_caret);
            lcd_stringout(old_caret == loc ? "X" : " ");

            // Print new caret location
            lcd_moveto(0, caret);
            lcd_stringout("^");

            // Set new old_caret location
            old_caret = caret;
        }

        // Delay
        _delay_ms(10);

        // Check if '^' is aligned with 'X'
        if(loc == caret) {
            // Print "You won!"
            if(count >= 200) {
                clear_lcd();
                lcd_moveto(0, 4);
                lcd_stringout("You won!");
                while(1) {}
            }
            else
                count++;
        }
        else {
            count == 0;
        }
    }

    return 0;   /* never reached */
}

