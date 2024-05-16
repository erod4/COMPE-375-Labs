#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

//rows and cols
#define ROWS 4
#define COLS 4


void keypad_scan(void);
void playSound(unsigned char r, unsigned char c);
void timer_init(unsigned char freq);


int main(void)
{
  //initialize GPIO pins
  //Data direction pins of port D 3-7 set to 1 for output (rows)
  DDRD |=(1<<DDD3)|(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);
  //Data directions pins of port B 0-3 set to 0 for input (cols)
  DDRB&=~(1<<DDB0)&~(1<<DDB1)&~(1<<DDB2)&~(1<<DDB3);
  //enable pull up for  input pins (cols)
  PORTB|=(1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3);
  //High Output for output pins
  PORTD|=(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
  while (1)
  {
    keypad_scan();
  }
}

void keypad_scan()
{
  //iterate through rows making each logic low and checking each column for a button press then send indices to transmit function
  for(unsigned char r=0; r<ROWS;r++)
  {
    //write logic low to row i+4 since rows are pins 4-7
    PORTD&=~(1<<(r+4));
    //commence cols check for button press
    for(unsigned char c=0; c<COLS;c++)
    {
      //button press is 0
      if(!(PINB&(1<<c)))
      {
        playSound(r,c);

      }
    }
    //set row back to logic high after check
    PORTD|=(1<<(r+4));

  }

}
void timer_init(unsigned char freq)
{
  TCCR0A|=(1<<WGM01);//set timer 0 to ctc mode
  OCR0A=freq; //set top value to the keys respective frequency
  OCR0B=(0.5)*freq; //set 50% duty cycle
  TCCR0B|=(1<<CS02);//set 256 pre-scale and start timer

}

void playSound(unsigned char r, unsigned char c)
{
  //will receive indices of key to play and array holds ocrb value
  const char keys[ROWS][COLS]={{141,133,126,118},
                 {112,105,99,94},
                 {88,83,79,74},
                 {70,66,62,59}};


  //create appropriate note
  timer_init(keys[r][c]); //pass frequency to timer_init to set timer OCR0A and OCR0B val


    while(!(PINB&(1<<c)))
    {
      //turn on sound
      PORTD|=(1<<PORTD3);
      //wait for OCR0B over flow and play key in the meantime
      while((TIFR0&(1<<OCF0B))==0);
      //clear overflow flag
      TIFR0|=(1<<OCF0B);

      //turn sound off
      PORTD&=~(1<<PORTD3);
      //wait for OCR0A overflow and keep sound off until then
      while((TIFR0&(1<<OCF0A))==0);
      //clear overflow flag
      TIFR0|=(1<<OCF0A);

    }


}





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
