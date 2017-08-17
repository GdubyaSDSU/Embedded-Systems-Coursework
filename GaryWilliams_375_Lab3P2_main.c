/*	Gary Williams
 *	COMPE 375 - 01
 *	Lab 3 P2  Due: 2/17/17
 *
 */
#define  F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#define	LED_ON PORTB |= (1<<PORTB5)
#define LED_OFF PORTB &= (0<<PORTB5)

void pause(int k){
	OCR0A = k;							//Set output Compare register to passed value
	TCCR0B |= (1<<CS02) | (1<<CS00);	//Set prescale to 1024 and start timer
	while(!(TIFR0 & (1<<OCF0A))){}		//Wait for compare flag in TIFR
	TCCR0B &= (0<<CS02) & (0<<CS00);	//Turn off the timer
	TIFR0 |= (1<<OCF0A);				//Reset the compare flag
}

void brighter(void){
	int inc;
	for(int i = 0; i < 100; i++){
		inc = i*156;
		inc = inc / 100;
		inc++;
		LED_ON;
		pause(inc);
		LED_OFF;
		pause(157-inc);	
	}
}

void dimmer(void) {
	int inc;
	for(int i = 1; i < 100; i++){
		inc = i*156;
		inc = inc / 100;
		inc++;
		LED_ON;
		pause(157-inc);
		LED_OFF;
		pause(inc);
	}
}

int main(void) {
	DDRB = 0x20;		//Setting port bit B5 to 1 for Output and port bit B7 0 for Input
	int wasPressed = 0;

	TCCR0A |= (1<<WGM01);		//Set timer to CTC mode
	
	while(1) {
		if(!(PINB & (1<<PINB7))) {			//Check to see if button pressed
			brighter();
			wasPressed = 1;
			while(!(PINB & (1<<PINB7))){	//Check to see button is still being held
				LED_ON;
			}
			LED_OFF;
		}
		else if(wasPressed) {				//Checking for flag to decrease
			dimmer();
			wasPressed = 0;
		}
	}
}