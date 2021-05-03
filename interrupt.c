/*
SW1, 2는 INT0,1에 그대로 연결
SEGMENT와 LED도 그대로 연결
SW1을 누르면 0 ~ 3까지 숫자가 카운트 되고, SW2를 누르면 LED ALL ONOFF를 10회(고정) 수행
SW1의 0 ~ 3은 EICRA 레지스터의 ISC 비트를 통해 해당값에 맞는 interrupt sense control 수행 (세부 제어는 EICRA 레지스터 설명 참조)
LED ALL ON/OFF 동작중 언제라도 SW1 또는 SW2를 누르면 동작 중지
각 IS 제어에 따른 스위치 동작에 대한 결과가 어떻게 달라지는 확인
*/

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define INT0_SW 1
#define INT1_SW 2

unsigned char seg_tbl[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x27, 0x7F, 0x67};
unsigned char in;
int count = 0;
int i = 0;

void disp_seg(int value)
{
	PORTD = seg_tbl[value] & 0xF0;
	PORTB = (seg_tbl[value] & 0x0F) | 0x30;
	PORTC = 0x3F;
}

int LED_ON_OFF()
{
	for (int j = 0; j < 10; j++)
	{
		if (in == INT0_SW)
		{
			break;
		}
		if (in == INT1_SW)
		{
			disp_seg(count);
			break;
		}
		PORTC = 0x00;
		PORTB &= ~0x30;
		_delay_ms(400);
		PORTC = 0xFF;
		PORTB = (PORTB & 0x0F) | 0x30;
		_delay_ms(400);
	}
}

int main()
{
	cli();
	DDRD = 0xF3;
	DDRB = 0x3F;
	DDRC = 0x3F;

	EICRA = 0x00;
	EIMSK = 0x03;
	EIFR = 0x03;
	disp_seg(EICRA & 0x03);
	sei();
	while (1)
	{
		if (in == INT1_SW)
		{
			in = 0;
			LED_ON_OFF();
			disp_seg(count);
			in = 0;
		}
	}
	return 0;
}

ISR(INT0_vect)
{
	_delay_ms(80);
	count = ++count % 4;
	if (count == 0)
	{
		EICRA = 0x00;
	}
	if (count == 1)
	{
		EICRA = 0x05;
	}
	if (count == 2)
	{
		EICRA = 0x0A;
	}
	if (count == 3)
	{
		EICRA = 0x0F;
	}
	disp_seg(count);
	in = INT0_SW;
	sei();
}
ISR(INT1_vect)
{
	_delay_ms(80);
	in = INT1_SW;
	sei();
}
