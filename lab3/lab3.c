/********************************************
*
*  Name: Haoda Wang
*  Email: haodawan@usc.edu
*  Lab section: 12:30 PM Friday
*  Assignment: Lab 3
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#define DOT_LENGTH  250         /* Define the length of a "dot" time in msec */

void dot(void);
void dash(void);
void makeOutput(char);
char checkInput(char);

int main(void)
{
    // Initialize appropriate DDR registers
    DDRB = 0x00;
    DDRD = 0x08;

    // Initialize the LED output to 0
    PORTD = 0x00;

    // Enable the pull-up resistors for the
    // 3 button inputs
	  PORTB = 0x38;

    // Loop forever
    while (1) {
	// Use if statements and the checkInput()
	//  function to determine if a button
	//  is being pressed and then output
	//  the correct dot/dash sequence by
	//  calling the dot(), dash(), and
	//  using appropriate delay functions

      if(!checkInput(5))
        makeOutput('u');
      else if (!checkInput(4))
        makeOutput('s');
      else if (!checkInput(3))
        makeOutput('c');

    }

    return 0;   /* never reached */
}

/*
  dot() - Makes the output LED blink a "dot".
  Write code to generate a dot by turning the
  output ON and OFF with appropriate delays.
  Be sure you don't forget to also delay an
  appropriate time after you turn the output off
*/
void dot()
{
  PORTD |= 0x08;  /* LED on */
  _delay_ms(DOT_LENGTH); /* Wait DOT_LENGTH milliseconds */
  PORTD &= 0x00; /* LED off */
  _delay_ms(100); /* Wait 100 milliseconds */
}

/*
  dash() - Makes the output LED blink a "dash".
  Write code to generate a dash by turning the
  output ON and OFF with appropriate delays.
  Be sure you don't forget to also delay
  appropriate time after you turn the output off
*/
void dash()
{
  PORTD |= 0x08;  /* LED on */
  _delay_ms(3 * DOT_LENGTH); /* Wait 2 * DOT_LENGTH milliseconds */
  PORTD &= 0x00; /* LED off */
  _delay_ms(100); /* Wait 100 milliseconds */
}

/*
  makeOutput() - Changes the output bit to either a zero
  or one, based on the input argument "value".

  If the argument is zero, turn the output OFF,
  otherwise turn the output ON.

  Do not use any delays here.  Just use bit-wise operations
  to make the appropriate PORT bit turn on or off.
*/
void makeOutput(char value)
{
  switch(value) {
  // ..- for 'u'
  case 'u':
    dot();
    dot();
    dash();
    break;
  // ... for 's'
  case 's':
    for(int i = 0; i < 3; i++)
      dot();
    break;
  // -.-. for 'c'
  case 'c':
    for(int i = 0; i < 2; i++) {
      dash();
      dot();
    }
    break;
  // Don't do anything in the default state
  default:
    break;
  }
}

/*
  checkInput(bit) - Checks the state of the input bit specified by the
  "bit" argument (0-7), and returns either 0 or 1 depending on its state.

  COMPLETE -- DO NOT CHANGE
*/
char checkInput(char bit)
{
    if ((PINB & (1 << bit)) != 0)
        return(1);
    else
        return(0);
}
