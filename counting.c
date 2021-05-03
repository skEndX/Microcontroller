/*
Segment 1: PD4 ~7, PB0 ~ 3
Segment 2: PB4 ,5, PC0 ~ 5
 
Action:
위 두 세그먼트를 이용하여 0 ~99까지 upcounting
다시 99에서 0까지 downcounting
무한반복
*/
#include <avr/io.h>
#define FOSC 16000000UL
#define F_CPU 16000000UL
#include <util/delay.h>

unsigned char seg_tbl[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x27, 0x7F, 0x67};
unsigned char seg_tbl1[] = {0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x10};

void main()
{
	DDRD = 0xF0;
	DDRB = 0x3F;
	DDRC = 0x3F;
	while (1)
	{
		for (int i = 0; i < 100; i++)
		{
			PORTD = 0xF0 & seg_tbl[i / 10];
			PORTB = (0x0F & seg_tbl[i / 10]) | (seg_tbl1[i % 10]);
			PORTC = 0x3F & seg_tbl[i % 10];
			_delay_ms(400);
		}
		for (int i = 99; i >= 0; i--)
		{
			PORTD = 0xF0 & seg_tbl[i / 10];
			PORTB = (0x0F & seg_tbl[i / 10]) | (seg_tbl1[i % 10]);
			PORTC = 0x3F & seg_tbl[i % 10];
			_delay_ms(400);
		}
	}
}
