#define  F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#define LED_ON PORTB|=(1<<PORTB5);
#define LED_OFF PORTB&=~(1<<PORTB5);

//assign initialized var into the EEPROM address space
uint8_t EEMEM duty_cycle=0;

void initTimer0();

int main(void)
{
  sei();

  //LED DD set to output
  DDRB |= (1<<5);

  initTimer0();



  uint8_t DC=eeprom_read_byte(&duty_cycle);
  //update OCR0B val

  OCR0B = (OCR0A * DC) / 100;
  //update duty cycle accordingly
  if(DC>=100)
  {
    DC=0;
  }
  else
  {
    DC+=10;
  }
  //store new duty cycle into EEPROM address 0
  eeprom_update_byte(&duty_cycle,DC);


    /* Replace with your application code */
    while (1) 
    {


    }
}



void initTimer0()
{
  TCCR0A |= (1<<WGM01); // Set timer to CTC mode
  OCR0A = 77;  // Set timer to compare at value for desired timing (5ms) ----> (0.005/(1/16MHz *256*1024))*256-1=77

  TIMSK0 |= (1<<OCIE0A)|(1<<OCIE0B); // Enable Timer0 compare match A interrupt

  TCCR0B |= (1<<CS02)|(1<<CS00); // Set prescaler to 1024 and start timer
}


ISR(TIMER0_COMPA_vect)
{
  LED_ON;	
}

ISR(TIMER0_COMPB_vect)
{
    LED_OFF;
}