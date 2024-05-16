/*
 * Lab02.c
 *
 * Created: 2/2/2024 5:47:42 PM
 * Author : erod7
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

//baud rate
#define UBRR F_CPU/16/9600-1

void init(unsigned int);
void sendData(char* redID);
void transmit(char n);

int main(void)
{
    /* Replace with your application code */
  //initialize UART
  init(UBRR);
  char redID[]="130248232\r\n";
    while (1) 
    {
    sendData(redID);
    _delay_ms(500);
    }
}

void init(unsigned int ubrr)
{
  //enable tx/rx pins
  UCSR0B|=(1<<RXEN0)|(1<<TXEN0);
  //set BAUD rate
  UBRR0H=(unsigned char) (ubrr>>8);
  UBRR0L=(unsigned char) ubrr;
  //set data frame
  //set 8 bit char size UCSZ02=0, UCSZ01=1, UCSZ00=1
  //stop bits already set to 1 bit (0)
  //parity bit by default is disabled UPM01=0, UPM00=0
  UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
}
void sendData(char* redID)
{
  //iterate through char of string and transmit each character
  for(int i=0; i<strlen(redID); i++)
  {
    transmit(redID[i]);	
  }

}
void transmit(char n)
{
  //check for buffer being clear 1=clear 0=not yet clear
  while(!(UCSR0A&(1<<UDRE0)));

  //transmit character
  UDR0=n;

}


