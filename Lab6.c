#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//rows and cols
#define ROWS 4
#define COLS 4
//LED control
#define LED_ON PORTB |= (1<<PORTB5);
#define LED_OFF PORTB &= ~(1<<PORTB5);
///RED_ID:130248232///
unsigned char key;
void keypad_scan(void);
void keypad_scan_timer(void);
void led_control_timer(void);
void GPIO_init(void);
int main(void)
{
  GPIO_init();
  keypad_scan_timer();
  led_control_timer();
  sei();  //enable interrupts

  while (1)
  {

  }
}

void keypad_scan()
{
  unsigned char keypad[ROWS][COLS]={{1,2,3,0},
  {4,5,6,0},
  {7,8,9,0},
  {0,0,0,0}};
  //iterate through rows making each logic low and checking each column for a button press then send indices to transmit function
  for(unsigned char r=0; r<ROWS;r++)
  {
    //write logic low to row i+4 since rows are pins 4-7
    PORTD&=~(1<<(r+4));
    //commence cols check for button press
    for(unsigned char c=0; c<COLS;c++)
    {
      //if button is pressed and the key is between 0-9
      if(!(PINB&(1<<c)))
      {
        key=keypad[r][c];
        OCR2B=OCR2A/10*key;  //set duty cyle to 0-90%

      }
    }
    //set row back to logic high after check
    PORTD|=(1<<(r+4));

  }

}

void keypad_scan_timer()
{
  //timer 0
  TCCR0A|=(1<<WGM01); //set timer to CTC mode
  OCR0A=186;  //set timer to 3ms (x+1 ms)
  TIMSK0|=(1<<OCIE0A); //set the ISR COMPA vect
  //set prescaler to 256 and start timer
  TCCR0B|=(1<<CS02);
}
void led_control_timer()
{
  //timer 2
  TCCR2A|=(1<<WGM21); //set timer to CTC mode
  OCR2A=155; //set timer to 2.5ms (z+1)*100hz=400 --> 400/1=2.5ms

  TIMSK2|=(1<<OCIE2A)|(1<<CS22);//set the ISR COMPA & COMPB vect
  //set prescaler to 256 and start timer
  TCCR2B|=(1<<CS22)|(1<<CS21);

}


ISR(TIMER0_COMPA_vect)
{
  //scan keypad
  keypad_scan();
}
ISR(TIMER2_COMPA_vect)
{
  if(key>0)
  {
    LED_ON;
  }

}
ISR(TIMER2_COMPB_vect)
{
  LED_OFF;
}
void GPIO_init()
{
  //initialize GPIO pins
  //Data direction pins of port D 4-7 set to 1 for output (rows)
  DDRD |=(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);
  //LED DD set to output
  DDRB |= (1<<DDB5);
  //Data directions pins of port B 0-3 set to 0 for input (cols)
  DDRB&=~(1<<DDB0)&~(1<<DDB1)&~(1<<DDB2)&~(1<<DDB3);
  //enable pull up for  input pins (cols)
  PORTB|=(1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3);
  //High Output for output pins
  PORTD|=(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
}
