#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void PutInt(int data) //Bluetooth function 1
{
	while(!(UCSR1A & 0x20)); UDR1 = data;
}

/* ISR(USART0_RX_vect) //Bluetooth function 2
{
	flag2 = UDR0;
} */

char RxChar(void)
{
	while(!(UCSR1A & 0x80)); return UDR1;
}

int main()
{
	DDRA = 0xff;

	//Bluetooth 
	UCSR1B = 0b00011000; //set, 만약에 안되면 10011000 이나 11011000 으로 바꿔라
	UCSR1C = 0b00000110; //set
	UBRR1H = 0;
	UBRR1L = 8; //Baud Rate > 115200
	SREG = 0x80;

	char flag2 = 'B';

	while(1)
	{
		flag2 = RxChar();

		if(flag2 == 'A')
		{
			PORTA = 0xff;
		}
		else if(flag2 == 'B')
		{
			PORTA = 0x00;
		}
		
		PutInt(123);
	}
	return 1;
}
