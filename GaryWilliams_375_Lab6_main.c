/*  Gary Williams
 *  COMPE 375-01
 *  Lab 6 Due: 3/10/17
 *
 *  Overview:
 *    For this laboratory assignment we used the same keypad as last lab but this
 *  time to make music tones and output them on a port that was connected to audio
 *  cable. We varied the frequency accordingly to what button on the keypad was 
 *  pressed. Initially, my code was truncated a division and I wasn't getting 
 *  varied tones until I divided by a float, then I got the variation that I 
 *  needed. I re-used my delay function from Lab 3 and used a 50% duty cycle on 
 *  the output frequency.
 *
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
