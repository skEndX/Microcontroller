#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define ON 1
#define OFF 0
int on_off = 0;
int note = 0;
unsigned int note_freq[7] = { 440,493,523,587,659,698,783 };
unsigned char led_array_PD[7] = {0x10,0x30,0x70,0xF0,0xF0,0xF0,0xF0};
unsigned char led_array_PB[7] = {0x00,0x00,0x00,0x00,0x01,0x03,0x07};

#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))

void play(unsigned int y)
{
   unsigned int x;
   x = FREQ(note_freq[y]);
   OCR1A = x;
}

void stop(void)
{
	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCNT1 = 0;
	OCR1A = 0;
	DDRB = ~0x02;
}

ISR (INT0_vect)
{
	cli();
	if(PIND&0b00001000)
	{
		stop();
   	}
	else
	{
		note++;
		_delay_ms(50);
		if(note >= 7)
		{
			note = 6;
		}		
		PORTD = ~(led_array_PD[note]) & 0xF0;
		PORTC = ~(led_array_PB[note]) & 0x0F;
		EIFR |= 0x01; 
	}
	sei();
}

ISR (INT1_vect)
{
	cli();
	if(PIND&0b00000100)
	{
		DDRB |= 0X02;	
   		TCCR1A |= 0x40;
   		TCCR1B |= 0x0A;
		EIFR |= 0x01; 
	}
	else
	{
		note--;
		_delay_ms(50);
		if(note < 0)
		{
			note = 0;
		}
		PORTD = ~(led_array_PD[note]);
		PORTC = ~(led_array_PB[note]);
		EIFR |= 0x02; 
	}
	sei();
}

int main(void)
{
	cli();
	DDRD = 0xF3;
	DDRB = 0x0F;
	DDRC = 0x0F;
	PORTD = 0x00;
	PORTB = 0x00;


	EICRA = 0x0A;
	EIMSK = 0x03;
	EIFR |= 0x03;

	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCNT1 = 0;
	OCR1A = 0;
	DDRB = ~0x02;
	sei();

	while(1)
	{
		play(note);
	}
}
