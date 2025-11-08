#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Ultrasound
#define TRIG_ON PORTB|=0x01
#define TRIG_OFF PORTB&=~0x01
#define ECHO PINE&0x80
volatile int distance;

volatile int flag2 = 0; //초음파 거리 측정 활성화 flag
volatile int sec = 0;

void Ultra() //Ultrasound function
{
	TRIG_OFF; 
	_delay_us(10); 
	TRIG_ON; 
	_delay_us(10); 
	TRIG_OFF;
	while(ECHO); 
	TCCR0 = 0x02; 
	TCNT0 = 0; //1/8
	while(!ECHO); 
	TCCR0 = 0x00; 
	distance = (int)TCNT0/116; //거리계산 수정
	_delay_ms(60);
}

/*
ISR(TIMER2_OVF_vect)
{
	sec++; //16.4ms

	if (sec == 30) // 
	{
		Ultra();

		if (distance < 25) //가까운 거리
		{
			for (int i=0; i<3; i++)
			{
					PORTG = 0x00; _delay_ms(400); 
					PORTG = 0x03; _delay_ms(100);
			}
		}
		else if ((distance >= 25)&&(distance < 75))  //비교적 먼 거리
		{
			for (int i=0; i<2; i++)
			{
				PORTG = 0x00; _delay_ms(400); 
				PORTG = 0x03; _delay_ms(400);
			}
		}
		
		PORTG = 0x00; sec = 0;
	}
} */


int main()
{
	//DDRA = 0xff;
	DDRG = 0x03;
	DDRB = 0x01;
	DDRE |= 0x80;

/*
	TIMSK = 0b01000001;
	TCNT2 = 0;
	TCCR2 = 0b00000101; //overflow interrupt mode, 1/1024(64us), 256*64us=16.4ms마다 interrupt
*/
	sei();

	while(1)
	{
		Ultra();

		if (distance < 25) //가까운 거리
		{
			for (int i=0; i<3; i++)
			{
					PORTG = 0x00; _delay_ms(400); 
					PORTG = 0x03; _delay_ms(100);
			}
		}
		else if ((distance >= 25)&&(distance < 75))  //비교적 먼 거리
		{
			for (int i=0; i<2; i++)
			{
				PORTG = 0x00; _delay_ms(400); 
				PORTG = 0x03; _delay_ms(400);
			}
		}
		
		PORTG = 0x00; 
		//sec = 0;

	}
	return 1;
}
