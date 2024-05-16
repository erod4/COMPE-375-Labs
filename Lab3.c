#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
//Calculate BAUD for UBRR
#define  UBRR F_CPU/16/9600-1
//rows and cols
#define ROWS 4
#define COLS 4

void init_UART(unsigned char ubrr);
void keypad_scan(void);
void transmit(unsigned char r, unsigned char c);



int main(void)
{
    //initialize UART
  init_UART(UBRR);
  //initialize GPIO pins
  //Data direction pins of port D 4-7 set to 1 for output (rows)
  DDRD |=(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);
  //Data directions pins of port B 0-3 set to 0 for input (cols)
  DDRB&=~(1<<DDB4)&~(1<<DDB5)&~(1<<DDB6)&~(1<<DDB7);
  //enable pull up for  input pins (cols)
  PORTB|=(1<<PORTB3)|(1<<PORTB2)|(1<<PORTB1)|(1<<PORTB0);
  //High Output for output pins
  PORTD|=(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
    while (1) 
    {
    keypad_scan();
    }
}

void init_UART(unsigned char ubrr)
{	//enable tx/rx
  UCSR0B|=(1<<RXEN0)|(1<<TXEN0);

  //set UBRR value into register
  //9600 fits into the lower 8 bits of register
  UBRR0H=(unsigned char) (ubrr>>8);
  UBRR0L=(unsigned char) ubrr;

  //8 bit transmission with 1 stop bit and no parity bit
  UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
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
        transmit(r,c);
        //de-bounce delay
        _delay_ms(50);
        //wait for button release
        while(!(PINB&(1<<c)));
        //de-bounce delay
        _delay_ms(50);
      }
    }
    //set row back to logic high after check
    PORTD|=(1<<(r+4));

  }	

}

void transmit(unsigned char r, unsigned char c)
{
  //will receive indices of char to transmit
  const char keypad[ROWS][COLS]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};

  //checking if buffer is empty before transmission
  while(!(UCSR0A&(1<<UDRE0)));

  //if while loop break then buffer is empty, transmit

  //place char into transmit buffer
  UDR0=keypad[r][c];

}



