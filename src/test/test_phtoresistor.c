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


//***함수

unsigned short read_adc() //Photoresistor function
{
   unsigned char adc_low, adc_high;
   unsigned short value; //16비트 강조(unsigned int와 같음)

   ADCSRA|=0x40; //0b01000000, ADC start conversion, ADSC=1
   
   while((ADCSRA&0x10)!=0x10); //ADCSRA & 0b00010000, ADC 변환 완료 검사(ADIF)

   adc_low=ADCL; //변환된 Low 값 읽어오기
   adc_high=ADCH; //변환된 High 값 읽어오기, 반드시 ADCL을 먼저 읽어야 함

   value = (adc_high<<8)|adc_low;
   return value;
}

//***메인

int main(void)
{
   //LED, Buzzer
   DDRA = 0xff; //LED, Buzzer
   PORTA = 0x00; //LED, Buzzer Initialization

   //Photoresist
   unsigned short value;
	DDRF = 0x00; //ADC initialization
   ADMUX = 0x40; //(REFS1,REFS0)=(0,1) > 기준 전압 AVCC(+5V), ADLAR=0 > 우측 정렬, MUX4~0 > 00000: ADC0 단일 채널 선택
   ADCSRA = 0x87; //0b10000111, ADEN=1 > AD 변환 기능 활성화, ADFR=0 > single conversion(한번만 변환) 모드, ADPS=111 > 프리스케일러 128분주(8us 주기)

   sei(); //open

   while(1)
   {  
      //Photoresist
      value = read_adc();
      if(value <= CDS_10) {PORTA=0x0a;} //10럭스 이하일 때 LED ON
      else {PORTA=0x00;} //LED OFF
   }
   return 1;
}
