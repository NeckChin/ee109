#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0 register

char buf[5];
short num_chars = 0;
char data_valid = 0;
short tog = 0;

char valid_data() {
  if(data_valid) {
    data_valid = 0;
    return 1;
  }
  else
    return 0;
}

char* remote_temp() {
  return buf;
}

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

void serial_tempout(short temp)
{
  char tem[3];
  snprintf(tem, 3, "%2d", abs(temp));
  serial_txchar('@');
  serial_txchar(temp < 0 ? '-' : '+');
  // Call serial_txchar in loop to send a string
  for(int i = 0; i < 2; i++) {
    serial_txchar(tem[i]);
  }
  serial_txchar('$');
}

ISR(USART_RX_vect)
{
  char ch;
  // Handle received character
  ch = UDR0;
  // Check if data start
  if(ch == '@') {
    tog = 0;
    num_chars = 0;
  }
  // Store in buffer
  else if(ch >= '0' && ch <= '9') {
    if(num_chars != 0) {
      buf[num_chars - tog] = ch;
      num_chars += 1;
    }
  }
  // Check if data end
  else if(ch == '$') {
    if(num_chars > 1)
      data_valid = 1;
  }
  else if(ch == '-' || ch == '+') {
    if(num_chars == 0) {
      if(ch == '-')
        buf[num_chars] = ch;
      else
        tog = 1;
      num_chars++;
    }
    else
      num_chars = 0;
  }
  else {
    num_chars = 0;
  }
}