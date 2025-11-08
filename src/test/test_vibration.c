#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//***함수

ISR(INT4_vect) //Vibration function
{
   PutInt(1);

   for(int i=0; i<10; i++) //5초동안 알림
   {
      PORTA = 0x00; _delay_ms(250); //LED, Buzzer OFF   
      PORTA = 0x81; _delay_ms(250); //LED, Buzzer ON
   }
   PORTA = 0x00; //OFF
}

//***메인

int main(void)
{
   //Vibration Sensor 
   EICRB |= 0xaa; //falling edge trigger
   EIMSK |= 0x20; //enable INT4
   EIFR |= 0xf0; //clear interrut flag

   sei(); //open

   while(1)
   {  
   }
   return 1;
}
