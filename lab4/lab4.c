/********************************************
*
*  Name: Haoda Wang
*  Email: haodawan@usc.edu
*  Section: Fri 12:30
*  Assignment: Lab 4 - Up/Down counter on LCD display
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

int main(void) {
    int space = 32;

    // Setup DDR and PORT bits for the 2 input buttons
    // as necessary
    DDRC = 0x00;
    PORTC = 0xff;

    // Initialize the LCD
    lcd_init();

    // Clear the LCD
    lcd_moveto(0, 0);
    for (int i = 0; i < 16; i++) lcd_stringout(&space);
    lcd_moveto(1, 0);
    for (int i = 0; i < 16; i++) lcd_stringout(&space);

    int direction = 0;    //0 for loop up, 1 for loop down
    int currentNum = 48;
    int count = 100;
        
    // Use a state machine approach to organize your code
    //   - Declare and initialize a variable to 
    //     track what state you are in        
    while (1) {               // Loop forever
        // Check for button pressed
        if ((PINC & (1 << PB4)) == 0)
            direction = 0;
        if ((PINC & (1 << PB2)) == 0)
            direction = 1;
        // Change number
        count++;
        if (count >= 100) {
            if (direction == 0) {
                if(currentNum >= 57)
                    currentNum = 47;
                currentNum++;
                lcd_stringout(&currentNum);
                lcd_moveto(0, 0);
            }
            else if (direction == 1) {
                if(currentNum <= 48)
                    currentNum = 58;
                currentNum--;
                lcd_stringout(&currentNum);
                lcd_moveto(0, 0);
            }
            count = 0;
        }
        _delay_ms(10);        // Delay before we go to the next iteration of the loop
    }

    return 0;   /* never reached */
}

