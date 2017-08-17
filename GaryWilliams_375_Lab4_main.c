/*  Gary Williams
 *  COMPE 375-01
 *  Lab 4 Due: 2/24/17
 *  
 *  Overview:
 *    For this laboratory assignment we were required to transmit the characters of our RedID
 *  through the USART port on the ATmega328PB. We were also required us to set the baud rate
 *  to 9600 and to set the data packet length to 8 data bits and 1 stop bit. After 
 *  transmitting the characters of our ID, we were to send a carriage return followed by a 
 *  linefeed and then wait 500 milliseconds before sending the string out again.
 *
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
