#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//***선언

//Photoresistor
#define CDS_10 341 //10럭스일 때 ADC 값, 341
#define CDS_20 450
#define CDS_50 550
#define CDS_80 650
#define CDS_100 731

//Vibration
volatile int flag = 0;

//Bluetooth
volatile char recep;

//Ultrasound
#define TRIG 0x80
#define ECHO (PINE&0x80)

//***함수

void PutInt(unsigned int data) //Bluetooth function 1
{
	while(!(UCSR1A & 0x20)); UDR1 = data;
}

void RxChar(void) //Bluetooth function 2
{
	while(!(UCSR1A & 0x80)); recep = UDR1;
}

ISR(INT4_vect) //Vibration function
{
	flag = 1;
}

unsigned short read_adc() //Photoresistor function
{
	unsigned char adc_low, adc_high;
	unsigned short value; 

	ADCSRA|=0x40; //ADC start conversion, ADSC=1
	
	while((ADCSRA&0x10)!=0x10);

	adc_low=ADCL;
	adc_high=ADCH;

	value = (adc_high<<8)|adc_low;
	return value;
}


//***메인

int main(void)
{
	//LED, Buzzer
	DDRA = 0xff; //LED, Buzzer
	PORTA = 0x00; //LED, Buzzer Initialization

	//Vibration Sensor 
	EICRB |= 0x02; //falling edge trigger
	EIMSK |= 0x10; //enable INT4
	EIFR |= 0x10; //clear interrut flag
	
	//Bluetooth 
	UCSR1B = 0b00011000; //set
	UCSR1C = 0b00000110; //set
	UBRR1H = 0;
	UBRR1L = 8; //Baud Rate > 115200
	SREG = 0x80;

	//Photoresist
	unsigned short value;
	DDRF = 0x00; //ADC initialization
	ADMUX = 0x40; //기준 전압 AVCC(+5V)
	ADCSRA = 0x87; //128분주(8us 주기)

	//Ultrasound
	DDRB |= 0x80;
	unsigned long distance;

	sei(); //open

	while(1)
	{
		//Photoresist
		value = read_adc();
		if(value <= CDS_10) {PORTA |= 0x0a;} //10럭스 이하일 때 LED ON
		else {PORTA &= ~0x0a;} //LED OFF

		//Vibration
		if (flag == 1)
		{
			PutInt(1);

			for(int i=0; i<10; i++) //5초동안 알림
			{
				PORTA = 0x00; _delay_ms(250); //LED, Buzzer OFF	
				PORTA = 0xaa; _delay_ms(250); //LED, Buzzer ON
			}
			PORTA = 0x00; flag = 0; //OFF
		} 
		else //Ultrasound
		{
    		PORTB|=0x80; _delay_us(10); PORTB&=~0x80; //TRIG
    		while(!ECHO); TCCR1B=2; TCNT1=0;
    		while( ECHO); TCCR1B=0; distance=(unsigned int)TCNT1/116;
			_delay_ms(10);

			if(distance <= 80) {PORTA &= ~0xa0; _delay_ms(150); PORTA |= 0xa0; _delay_ms(150); PORTA &= ~0xa0;}
			else if (distance <= 210) {PORTA |= 0x20; _delay_ms(300);}
			else {PORTA &= ~0x20; _delay_ms(300);}
		}
	}
	return 1;
}