/*  Gary Williams
 *  COMPE 375 - 01
 *  Lab 3  Due: 2/17/17
 *
 *  Overview:
 *    For this laboratory assignment we were to generate a PWN waveform to
 *  drive the LED so that when the button is pressed, the duty cycle increases
 *  from 0% to 100% and decreases back to 0% when the button is released. For 
 *  the first part of this lab, we were to use the built in delay functionality
 *  to generate the pulse variances. The hardest part about using the built in
 *  delay is that it would not take a variable as an argument. So a separate
 *  delay function was made and just called the delay as many times as 
 *  specified. It seems like this method would have unnecessary overhead, but
 *  it actually worked quite well and the light up and light down looked smooth.
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
