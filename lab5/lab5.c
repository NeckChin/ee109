/********************************************
*
*  Name: Haoda Wang
*  Email: haodawan@usc.edu
*  Section: Fri 12:30
*  Assignment: Lab 5 - Number Lock
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

// Write lock state on LCD
void write_state(int state) {
    lcd_moveto(0, 0);
    char s[10];
    sprintf(s, "State=S%d", state);
    lcd_stringout(s);
}

// Write lock status on LCD
void write_status(int status) {
    lcd_moveto(1, 0);
    lcd_stringout(status == 1 ? "Status=Unlocked" : "Status=Locked");
}

// Clear the LCD
void clear_lcd() {
    int space = 32;
    lcd_moveto(0, 0);
    for (int i = 0; i < 16; i++) lcd_stringout(&space);
    lcd_moveto(1, 0);
    for (int i = 0; i < 16; i++) lcd_stringout(&space);
}

int main(void) {
    // Setup DDR and PORT bits for the 2 input buttons
    // as necessary
    DDRC = 0x00;
    PORTC = 0xff;

    // Initialize the LCD
    lcd_init();

    // Clear the LCD
    clear_lcd();

    // Display splash screen for a second
    lcd_moveto(0,3);
    lcd_stringout("Haoda Wang");
    char date[9];
    lcd_moveto(1,3);
    sprintf(date, "%d/%d/%d", 3, 13, 2000);
    lcd_stringout(date);

    _delay_ms(1000);

    // Clear the LCD
    clear_lcd();

    // Control values
    int state = 0;
    int status = 0;
    
    // Use a state machine approach to org`anize your code
    //   - Declare and initialize a variable to 
    //     track what state you are in        
    while (1) {               // Loop forever
        // Check if unlocked
        if(state == 4) {
            status = 1;
        }

        // Output status on LCD
        write_state(state);
        write_status(status);

        // If button A pressed
        if ((PINC & (1 << PB4)) == 0) {
            if(state == 0 || state == 3)
                state++;
            else if(state == 2)
                state = 1;
            if(status == 1) {
                // Clean up and reset
                state = 0;
                status = 0;
                // Clear LCD so it doesn't say "Lockeded"
                clear_lcd();
            }
            // Debounce button
            while((PINC & (1 << PB4)) == 0) {}
        }
        // If button B pressed
        if ((PINC & (1 << PB2)) == 0) {
            if(state == 1 || state == 2)
                state++;
            else if(state == 3)
                state = 0;
            if(status == 1) {
                // Clean up and reset
                state = 0;
                status = 0;
                // Clear LCD so it doesn't say "Lockeded"
                clear_lcd();
            }
            // Debounce button
            while((PINC & (1 << PB2)) == 0) {}
        }

        _delay_ms(10);        // Delay before we go to the next iteration of the loop
    }

    return 0;   /* never reached */
}