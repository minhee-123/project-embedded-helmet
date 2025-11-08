#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//***선언

//Ultrasound
#define blank 99
#define SR04_Trigger 0x80 //PB7
#define SR04_Echo 0x80  //PE7
#define Trigger_ON PORTB |= SR04_Trigger
#define Trigger_OFF PORTB &= ~SR04_Trigger
#define Echo (PINE & SR04_Echo)

//***함수

void ultraTrig(void)
{
	Trigger_ON;
     _delay_us(10);
     Trigger_OFF;
}

unsigned int getEcho(void)
{
   unsigned int range_I;
   float range_F;
   unsigned int range1, range2;
   static unsigned int cnt = 0;
   cnt = 0;
   while(!Echo)
   {
      cnt++;
      _delay_us(1);
      if(cnt >= 1000)
      {
         cnt = 0;
         break;
      }
   }
   TCNT1 = 0;
   TCCR1B = 2; //8분주
   cnt = 0;
   while(Echo)
   {
      cnt++;
      _delay_us(1);
      if(cnt >= 1000)
      {
         cnt = 0;
         break;
      }
   }
   TCCR1B = 8;
   range_I = TCNT1;
   range_F = (float) range_I;
   range_F = range_F/116;
   range1 = (unsigned int) range_F;
   range2 = range1;
   return range2;
}

//***메인

int main(void)
{
   //LED, Buzzer
   DDRA = 0xff; //LED, Buzzer
   PORTA = 0x00; //LED, Buzzer Initialization

	//Ultrasound	
   unsigned int distance = 0;
   DDRE = 0x10;
   TCCR1A = 0;
   TCCR1B = 8;

   sei(); //open

   while(1)
   {  
      ultraTrig();
      distance = getEcho();
	  _delay_ms(10)

      if (distance <= 15) //가까운 거리
        {
        // for (int i=0; i<3; i++)
        // {
            PORTA = 0x00; //_delay_ms(400); 
           // PORTA = 0xff; _delay_ms(100);
        // }
       }
       else if ((distance <= 30))  //비교적 먼 거리
       {
       //  for (int i=0; i<2; i++)
       //  {
            PORTA = 0x00; //_delay_ms(400); 
            //PORTA = 0xff; _delay_ms(400);
        // }
       }
       else PORTA = 0xff;
   }
   return 1;
}
