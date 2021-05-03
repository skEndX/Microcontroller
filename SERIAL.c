#include<avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define	BAUD_9600	103
unsigned char send_char = 0;
#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))
unsigned int note_freq[7] = { 440,493,523,587,659,698,783 };


void play(unsigned int y)
{
		DDRB |= 0X02;	
   		TCCR1A |= 0x40;
   		TCCR1B |= 0x0A;
		EIFR |= 0x01;
   unsigned int x;
   x = FREQ(note_freq[y]);
   OCR1A = x;
}



int main()
{
	cli();
	DDRD = 0xF3;
	DDRB = ~0x02;
	DDRC = 0x0F;
	PORTD = 0x00;
	PORTB = 0x00;

	TCCR1A = 0x00;
	TCCR1B = 0x00;
	TCNT1 = 0;
	OCR1A = 0;

	UBRR0H = (unsigned char) (BAUD_9600 >>8) ;
	UBRR0L = (unsigned char) BAUD_9600;
	UCSR0B = (1<< TXEN0)|  (1<< RXEN0);	
	UCSR0C = (1<< UCSZ01) | (1<< UCSZ00);
	sei();
	while(1){ 
		
		if(UDR0 != 0){
			while (!(UCSR0A & 1<<RXC0));
			send_char = UDR0;
			if(send_char == 'A') send_char = 0;
			if(send_char == 'S') send_char = 1;
			if(send_char == 'D') send_char = 2;
			if(send_char == 'F') send_char = 3;
			if(send_char == 'Z') send_char = 4;
			if(send_char == 'X') send_char = 5;
			if(send_char == 'C') send_char = 6;
			play(send_char);
			send_char = 0;
		}
	}
}
