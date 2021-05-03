#include <avr/io.h>
#define F_CPU 16000000UL
#define FOSC 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

void LED_BLINK(void)
{
	PORTD = 0x00;
	_delay_ms(300);
	PORTD = 0xFF;
}

ISR(TIMER1_OVF_vect)
{
	cli();
	TCNT1 = 0xFFFF - 0xF424;
	LED_BLINK();
	sei();
}

int main(void)
{
	cli();
	DDRD = 0xFF;
	PORTD = 0xFF;


	TCNT1 = 0xFFFF - 0xF424;
	TCCR1A = 0;
	TCCR1B |= 0b00000100;
	

	TIMSK1 |= 1;
	TIFR1 |= 1;

	sei();

	while(1)
	{
		
	}
}
