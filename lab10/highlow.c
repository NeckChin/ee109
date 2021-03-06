#include <avr/io.h>
#include <avr/interrupt.h>

char changed = 0;

char old_state, new_state;
char a, b;

unsigned char cold_hot = 0;
unsigned char toggle_changed = 0;
short high_temp = 80;
short low_temp = 60;

void encoder_init() {
  // Read the A and B inputs to determine the intial state
  unsigned char input = PINC;
  a = input & (1 << PC1);
  b = input & (1 << PC4);
  if (a && b)
    old_state = 3;
  else if (!a && !b)
    old_state = 0;
  else if (!a && b)
    old_state = 2;
  else
    old_state = 1;
}

void increase_temp(short diff) {
  if(cold_hot) {
    low_temp += diff;
    if(low_temp < 40)
      low_temp = 40;
    else if(low_temp > 100)
      low_temp = 100;
    else if(low_temp > high_temp)
      low_temp = high_temp;
  }
  else {
    high_temp += diff;
    if(high_temp < 40)
      high_temp = 40;
    else if(high_temp > 100)
      high_temp = 100;
    else if(high_temp < low_temp)
      high_temp = low_temp;
  }
}

char temp_has_change() {
  if(changed == 1) {
    changed = 0;
    return 1;
  }
  else
    return 0;
}

ISR(PCINT1_vect) {
  short temp_diff = 0;
  // Read the input bits and determine A and B
  unsigned char input = PINC;
  a = input & (1 << PC1);
  b = input & (1 << PC4);

  // For each state, examine the two input bits to see if state
  // has changed, and if so set "new_state" to the new state,
  // and adjust the temp value.
  if (old_state == 0) {
    // Handle A and B inputs for state 0
    if (a) {
      new_state = 1;
      temp_diff++;
    }
    else if (b) {
      new_state = 2;
      temp_diff--;
    }
  }
  else if (old_state == 1) {
    // Handle A and B inputs for state 1
    if (!a) {
      new_state = 0;
      temp_diff--;
    }
    else if (b) {
      new_state = 3;
      temp_diff++;
    }
  }
  else if (old_state == 2) {
    // Handle A and B inputs for state 2
    if (a) {
      new_state = 3;
      temp_diff--;
    }
    else if (!b) {
      new_state = 0;
      temp_diff++;
    }
  }
  else {   // old_state = 3
    // Handle A and B inputs for state 3
    if (!b) {
      new_state = 1;
      temp_diff--;
    }
    else if (!a) {
      new_state = 2;
      temp_diff++;
    }
  }

  increase_temp(temp_diff);

  // If state changed, update the value of old_state
  if (new_state != old_state) {
    old_state = new_state;
    changed = 1;
  }
}

ISR(PCINT0_vect) {
  if(!(PINB & (1 << PB3)))
    cold_hot = 0;
  else if(!(PINB & (1 << PB4)))
    cold_hot = 1;
}