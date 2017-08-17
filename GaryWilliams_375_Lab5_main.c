/*  Gary Williams
 *  COMPE 375-01
 *  Lab 5 Due: 3/3/17
 *
 *  Overview:
 *    For this laboratory assignment we were to scan the 4x4 keyboard we were given and
 *  output the character that was pressed. The way the keyboard is set up, we output a 
 *  signal on the rows and read the signal in from the columns. One at a time, we go 
 *  through the rows and check the columns to see which pin has dropped to low to 
 *  indicate a button press. Because we know which row and column we are in, I called a
 *  2D array with the characters in the order on the keypad for output.  We used the 
 *  USART serial port to output the character and used the terminal within Atmel Studio
 *  to view the results.
 *
 */
#define F_CPU 16000000UL
#define BAUDRATE ((F_CPU)/(ubrr*16UL)-1)

#include <avr/io.h>
#include <util/delay.h>

void USART_init(int ubrr);
void USART_Tx_char(char key);


int main(void)
{
	char keypad[4][4]= {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
	int i, j;
	unsigned char row[] = {0xEF, 0xDF, 0xBF, 0x7F};
	USART_init(9600);
	
	DDRD = 0xF0;
	DDRB = 0x00;
	PORTB = 0x0F;
	
	while(1){
		PORTD = 0xFF;
		for(i=0; i<4; i++){						//One at a time turn them off
			PORTD = row[i];
			for (j=0; j<4; j++){
				if(!(PINB & (1<<j))){
					_delay_ms(50);
					if(!(PINB & (1<<j))){
						USART_Tx_char(keypad[i][j]);
						while(!(PINB & (1<<j)));
					}
				}
			}
		}
	}
	
	return 0;
}

void USART_init(int ubrr){
	UBRR0H = (BAUDRATE>>8);					//Set the baudrate to what was passed
	UBRR0L = BAUDRATE;	
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);	//Set data size to 8 bit; default 1 stop bit
}

void USART_Tx_char(char key){
	UCSR0B |= (1<<TXEN0);					//Set USART to transmit
	while(!(UCSR0A & (1<<UDRE0)));			//Wait for the data register to be free
	UDR0 = key;								//Send passed char
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = 0x20;							//Send line feed
	while(!(UCSR0A & (1<<UDRE0)));
	UCSR0B &= (0<<TXEN0);					//Turn off USART
}
