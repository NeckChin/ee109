#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

char buf[5];
short num_chars = 0;
char data_valid = 0;

#define FOSC 16000000           // Clock frequency
#define BAUD 9600               // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0 register

char valid_data() {
  if(data_valid) {
    data_valid = 0;
    return 1;
  }
  else
    return 0;
}

short remote_temp() {
  int temp = 0;
  sscanf(buf, "%d", &temp);
  return temp;
}

void serial_init()
{
  UBRR0 = MYUBRR;
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
  char tem[4];
  snprintf(tem, 4, "%d", abs(temp));
  serial_txchar('@');
  serial_txchar(temp < 0 ? '-' : '+');
  // Call serial_txchar in loop to send a string
  if(temp >= 100)
    for(int i = 0; i < 3; i++)
      serial_txchar(tem[i]);
  else
    for(int i = 0; i < 2; i++)
      serial_txchar(tem[i]);
  serial_txchar('$');
}

ISR(USART_RX_vect)
{
  char ch;
  // Handle received character
  ch = UDR0;
  // Check if data start
  if(ch == '@') {
    num_chars = 0;
    data_valid = 0;
  }
  // Check if data end
  else if(ch == '$') {
    if(num_chars > 1)
      data_valid = 1;
  }
  // Store in buffer
  else if(ch >= '0' && ch <= '9') {
    if(num_chars >= 1) {
      buf[num_chars] = ch;
      num_chars += 1;
    }
    else {
      num_chars = 0;
    }
  }
  else if(ch == '-' || ch == '+') {
    if(num_chars == 0) {
      buf[num_chars] = ch;
      num_chars++;
    }
    else
      num_chars = 0;
  }
  else {
    num_chars = 0;
  }
}