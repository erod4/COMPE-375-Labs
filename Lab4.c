#define F_CPU 16000000UL // 16MHz clock from the debug processor
#include <avr/io.h>
#include <util/delay.h>


void LED_ON();
void LED_OFF();


int main(void)
{
  //I/O initialization
  DDRB|=(1<<DDB5);//dd for led set to output
  DDRB&=~(1<<DDB7);// dd of button set to input
  PORTB|=(1<<PORTB7);//enable pu resistor
  //Timer initialization
  TCCR0A|=(1<<WGM01); //set to ctc mode WGM02=0 WGM01=1 WGM00=0 but initially these 3 pins start at 0
  OCR0A=155; //set top to 155 = 10ms (100% duty cycle)
  OCR0B=0;//set to 0% duty cycle
  TCCR0B|=(1<<CS02)|(1<<CS00); //set prescale to 1024 and start timer

    while (1) 
    {	
    if(!(PINB&(1<<PINB7))&&(OCR0B<OCR0A)) //if button is pressed and duty cycle is less than 100% increase duty cycle
    {
        OCR0B++;
    }	
    LED_ON();
    while(!(TIFR0&(1<<OCF0B))); //wait for overflow event
    TIFR0|=(1<<OCF0B);//reset overflow flag by writing 1 to it

    if((PINB&(1<<PINB7))&&(OCR0B>0))//if button is not pressed and duty cycle is greater than 0% decrease duty cycle
    {
      OCR0B--;
    }
    LED_OFF();
    while(!(TIFR0&(1<<OCF0A)));//wait for overflow event
    TIFR0|=(1<<OCF0A); //reset overflow flag by writing 1 to it

  }
}

void LED_ON()
{
  PORTB|=(1<<PORTB5);
}

void LED_OFF()
{
  PORTB&=~(1<<PORTB5);
}

