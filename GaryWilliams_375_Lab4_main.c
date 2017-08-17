/*	Gary Williams
 *	COMPE 375-01
 *	Lab 4 Due: 2/24/17
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

void USART_init(void);

int main (void)
{
	char redid[]= {'8','0','3','1','6','2','4','9','7'};
	USART_init();
	UCSR0B |= (1<<TXEN0);					//Set USART to transmit
	
	while(1){
		for(int i=0; i<sizeof(redid); i++){
			while(!(UCSR0A & (1<<UDRE0)));	//Wait for the data register to be free
			UDR0 = redid[i];
		}
		while(!(UCSR0A & (1<<UDRE0)));	
		UDR0 = 0x0D;						//Send carriage return
		while(!(UCSR0A & (1<<UDRE0)));	
		UDR0 = 0x0A;						//Send line feed
		while(!(UCSR0A & (1<<UDRE0)));
		_delay_ms(500);						//Delay between transmission strings
	}
	
	return 0;
}

void USART_init(void){
	UBRR0L = 103;							//Set the baudrate to 9600
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);	//Set data size to 8 bit; default 1 stop bit
}