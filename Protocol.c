#include<avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define   BAUD_9600   103
#define   MAX_BUFF_SIZE   128
#define LED_ON_OFF 0x01
#define LED_ALT 0x02
#define LED_SHIFT 0x04
#define Segment_Up 0x08
#define Buzzer 0x10
#define STX 0x02
#define ETX 0x03
#define FREQ(x) (unsigned int)(16000000/(2*8*(1+x)))

unsigned char seg_arr[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
//unsigned char seg_arr2[] = {0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};

int decode = 0;

uint8_t _command = 0;
unsigned char _data = 0;

void Printf(char *fmt, ...)
{
   va_list arg_ptr;
   uint8_t i,len;
   char sText[128];
      
   /* text buffer */
   for (i = 0; i < 128; i++)
      sText[i] = 0;

   va_start(arg_ptr, fmt);
   vsprintf(sText, fmt, arg_ptr);
   va_end(arg_ptr);

   len = strlen(sText);
   for (i = 0; i < len; i++){
      tx_char(sText[i]);
   }
}


void hex_command(uint8_t cd, unsigned char data)
{
   if(cd==LED_ON_OFF)
   {
      LED_ONOFF(data,400);
   }
   else if(cd==LED_ALT)
   {
      LEDALT(data,400);
   }
   else if(cd==LED_SHIFT)
   {
      LEDSHIFT(data);
   }
   else if(cd==Segment_Up)
   {
      disp_seg(data);
   }
   else if(cd==Buzzer)
   {
      if(data==0x01)
      {
         buzzer(1);
      }
      else if(data==0x00)
      {
         buzzer(0);
      }

   }
   else
   {
      Printf("Invalid Command\n");
   }
}

unsigned char _buff[MAX_BUFF_SIZE]; 
uint8_t com_size = 0; 
int sint_flag;

void tx_char(unsigned char Rx_char)
{
   while(!(UCSR0A & (1<<UDRE0)));
   UDR0 = Rx_char;
}

void LED_ONOFF(unsigned char num, int sec)
{
   for(int i=0;i<num;i++)
   {
   PORTB = (PORTB & ~(0x3C)) | 0xFF;
   PORTD = (PORTD & ~(0xF0)) | 0xFF;
   _delay_ms(sec);

   PORTB = (PORTB & ~(0x3C)) | 0x00;
   PORTD = (PORTD & ~(0xF0)) | 0x00;
   _delay_ms(sec);
   }

}

void LEDALT(unsigned char num, int sec)
{
   for(int i=0;i<num;i++)
   {
   PORTB = (PORTB & ~(0x3C)) | 0x55;
   PORTD = (PORTD & ~(0xF0)) | 0x55;
   _delay_ms(sec);

   PORTB = (PORTB & ~(0x3C)) | 0xAA;
   PORTD = (PORTD & ~(0xF0)) | 0xAA;
   _delay_ms(sec);
   }
}

void LEDSHIFT(unsigned char num)
{
   for(int i=0;i<num;i++)
   {
      for(int j=0;j<8;j++)
      {
      PORTB = (PORTB & ~(0x3C)) | (~((0x01<<2)<<j));
      PORTD = (PORTD & ~(0xF0)) | (~(0x01<<j));
      _delay_ms(200);
      }
      for(int k=7;k>=0;k--)
      {
      PORTB = (PORTB & ~(0x3C)) | (~((0x01<<2)<<k));
      PORTD = (PORTD & ~(0xF0)) | (~(0x01<<k));
      _delay_ms(200);
      }
   }
}

void disp_seg(int num)
{
   for(int i=0;i<=num;i++)
   {
      //if(i>=8)
      //{
         PORTC = (PORTC & ~(0x3F)) | (seg_arr[i]&0x3F);
         PORTD = (PORTD & ~(0x0C)) | ((seg_arr[i]&0xC0)>>4);
         _delay_ms(400);
//      }
      //PORTC = (PORTC & ~(0x3F)) | (seg_arr[i]&0x3F);
      //PORTD = (PORTD & ~(0x0C)) | (seg_arr[i]&0xC0)>>4;
      //_delay_ms(400);
   }
}

void buzzer(int i)
{
   if(i==1)
   {
      unsigned int x;
        DDRB |= 0x02;
        TCCR1A |= 0x40;
        TCCR1B |= 0x1A;
        x = FREQ(1000);
        ICR1 = x;
   }
   else
   {
      TCCR1A = 0x00;
         TCCR1B = 0x00;
         TCNT1 = 0;
         ICR1 = 0;
         DDRB = ~0X02;
   }
}



void com_puts(uint8_t *buff,uint8_t len)
{
   int j = 0;
   for(j=0;j<len;j++)
   {
   while (!(UCSR0A & 1 << UDRE0));
   UDR0 = buff[j];
   }
}

int main()
{
   cli();
   uint8_t buf[MAX_BUFF_SIZE];
    DDRD = 0xFF;
   DDRB = 0xFF;
   DDRC = 0xFF;
   //EICRA = 0xAA;    
   //EIMSK = 0x03;    
   //EIFR |= 0x0F;   
   
    
   UBRR0H = (unsigned char) (BAUD_9600 >>8) ;
   UBRR0L = (unsigned char) BAUD_9600;
   UCSR0B = (1<< TXEN0)|  (1<< RXEN0);   
   UCSR0B |=(1<<RXCIE0);
   UCSR0C = (1<< UCSZ01) | (1<< UCSZ00);

   sei();

     while(1){ 
   //LEDALT(3,400);
   //LEDSHIFT(3);
//   disp_seg(10);
   //disp_seg(15);

   //   disp_seg(16);

    if(sint_flag){ 
      hex_command(_command,_data);
      sint_flag = 0;
      _command = 0;
      _data = 0;
        }   
    }
}
   



ISR(USART_RX_vect)
{
   unsigned char _usart_rcv = 0;
   _usart_rcv = UDR0;

   if(decode == 0 && _usart_rcv == STX){
      decode =1;
      //Printf("aa");
   }else if(decode ==1){
      _command = _usart_rcv;
      decode = 2;
      //Printf("bb");
   }
   else if(decode == 2){
      _data = _usart_rcv;
      decode = 3;
      //Printf("cc");
   }
   else if(decode == 3){
      if(_usart_rcv == ETX){
         sint_flag = 1;
      }else{
         Printf("RX error\n");
      }
      decode = 0;
   }
   sei();

}
