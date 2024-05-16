/*
 * Lab0.c
 *
 * Created: 1/26/2024 4:13:46 PM
 * Author : erodriguez1091
 */ 

#define F_CPU 16000000UL // 16MHz clock from the debug processor
#include <avr/io.h>
#include <util/delay.h>
#include <ctype.h>
#include <string.h>

void dot();
void dash();
void space(uint8_t choice);
void to_morse(uint8_t index);
void morse(char* c);

int main(void)
{
  DDRB |= (1<<DDB5); // Set port bit B5 in data direction register to 1:(output)
  char str[]="Enrique 130248232";
  while(1){
    morse(str);
  }

}


void dash(){
  PORTB |= (1<<PORTB5); //Set port bit B5 to 1 to turn on the LED
  _delay_ms(600); 
  PORTB &= ~(1<<PORTB5); //Clear port bit B5 to 0 to turn off the LED

}

void dot(){
  PORTB |= (1<<PORTB5); //Set port bit B5 to 1 to turn on the LED
  _delay_ms(200); 
  PORTB &= ~(1<<PORTB5); //Clear port bit B5 to 0 to turn off the LED
}

void space(uint8_t choice){
  switch(choice)
  {
    case 1:
    _delay_ms(200);
    break;
    case 2:
    _delay_ms(600);
    break;
    case 3:
    _delay_ms(1400);
    break;
  }
}
void morse(char* c){
  //iterate through string
  int indx;
  for(uint8_t i=0; i<strlen(c);i++)
  {
    //check for a space
    if(c[i]==' ')
    {
      space(3);
    }
    else{
      //otherwise it can be a # or letter so we need to check
      //check to see if the ascii value is a letter or number
      if(toupper(c[i])-'A'>=0 && toupper(c[i])-'A'<26)
      {
        //if this condition is met then this means it must be a letter
        indx=toupper(c[i])-'A';
        to_morse(indx);
      }
      else if(isdigit(c[i]))
      {
        //if this condition is met then this means that it must be a #
        //+26 to get to the numbers
        indx=26+c[i]-'0';
        to_morse(indx);
      }
    }
  }
}


void to_morse(uint8_t indx)

{
//a,b,c,d,e,f,...,z,0,1,2,3,...,9 (in morse)
  char* morse[]={".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--..","-----",".----","..---","...--","....-",".....","-....","--...","---..","----."};

  for (uint8_t i=0; i<strlen(morse[indx]); i++)
  {
    if(morse[indx][i]=='.')
    {
      dot();
      if(morse[indx][i+1]!='\0'&&morse[indx][i+1]=='.')
      {
        space(1);
      }
      else if(morse[indx][i+1]!='\0'&&morse[indx][i+1]=='-')
      {
        space(2);

      }	
    }
    else
    {
      //must equal a dash
      dash();
      //check if following char is dot or dash
      //check if the following char is a dash or dot and call a space function accordingly
      if(morse[indx][i+1]!='\0'&&morse[indx][i+1]=='-')
      {
        space(1);

      }
      else if(morse[indx][i+1]!='\0'&&morse[indx][i+1]=='.')
      {
        space(2);

      }

    }

  }
}

