/*	Gary Williams
 *	COMPE 375 - 01
 *	Lab 3  Due: 2/17/17
 *
 */
#define  F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#define	LED_ON PORTB |= (1<<PORTB5)
#define LED_OFF PORTB &= (0<<PORTB5)

void delay(int x) {
	x = x * 100;
	for(int i = 0; i < x; i++) _delay_us(1);
}

void brighter(void){
	for(int i = 0; i < 100; i++){
		LED_ON;
		delay(i);
		LED_OFF;
		delay(100-i);		
	}
}

void dimmer() {
	for(int i = 0; i < 100; i++){
		LED_ON;
		delay(100-i);
		LED_OFF;
		delay(i);
	}
}

int main(void) {
	DDRB = 0x20;		//Setting port bit B5 to 1 for Output and port bit B7 0 for Input
	int wasPressed = 0;
	
	while(1) {
		if(!(PINB & (1<<PINB7))) {			//Check to see if button pressed
			brighter();
			wasPressed = 1;
			while(!(PINB & (1<<PINB7))){	//Check to see button is still being held
				LED_ON;
			}
		}
		else if(wasPressed) {				//Checking for flag to decrease
			dimmer();
			wasPressed = 0;
		}
	}
}
