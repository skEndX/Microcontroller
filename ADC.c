#include <avr/io.h>
#define FOSC 16000000UL
#define F_CPU 16000000UL
#include <util/delay.h>
unsigned char seg[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x67};
int count = 0;

void main(void)
{
	DDRD = 0xF0;
	DDRB = 0x3F;
	DDRC = 0x3F;

	while(1)
	{
		for(int i = 0; i< 100; i++)
		{
			PORTB = (PORTB &0x0F)|0x30&(seg[i%10]<<4);
			PORTC = 0x3F&(seg[i%10]>>2);
			PORTB = (PORTB &0x30)|0x0F&(seg[i/10]>>4);
			PORTD = 0xF0&(seg[i/10]<<4);
			_delay_ms(100);		
		}
		for(int i = 98; i>=1; i--)
		{
			PORTB = (PORTB &0x0F)|0x30&(seg[i%10]<<4);
			PORTC = 0x3F&(seg[i%10]>>2);
			PORTB = (PORTB &0x30)|0x0F&(seg[i/10]>>4);
			PORTD = 0xF0&(seg[i/10]<<4);
			_delay_ms(100);	
		}
	}
}
