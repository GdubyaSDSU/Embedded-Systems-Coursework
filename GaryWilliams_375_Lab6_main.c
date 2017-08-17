/*	Gary Williams
 *	COMPE 375-01
 *	Lab 6 Due: 3/10/17
 */
#define F_CPU 16000000UL
#define BAUDRATE ((F_CPU)/(ubrr*16UL)-1)

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

void delay(int x);

int main(void)
{
	int keypad[4][4]= {{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}};
	int i, j;
	unsigned char row[] = {0xEF, 0xDF, 0xBF, 0x7F};
    int halfTotal;
	
	DDRD = 0xF0;
	DDRB = 0x00;
	DDRC = 0x01;
	PORTB = 0x0F;
		
	while(1){
		PORTD = 0xFF;					//Turn on all D ports
		for(i=0; i<4; i++){				//One at a time turn them off
			PORTD = row[i];
			for (j=0; j<4; j++){
				if(!(PINB & (1<<j))){
					_delay_ms(50);
					if(!(PINB & (1<<j))){
						halfTotal =1/(pow(2,(keypad[i][j]/12.)) * 440)*500000;
						while(!(PINB & (1<<j))){
							PORTC |= 1;
							delay(halfTotal);
							PORTC &= 0;
							delay(halfTotal);
						}
					}
				}
			}
		}
	}
	
	return 0;
}

void delay(int x) {
	for(int i = 0; i < x; i++) _delay_us(1);
}
