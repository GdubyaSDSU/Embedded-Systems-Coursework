/* Gary Williams
 * COMPE 375 - 01
 * Lab 2  Due: 2/10/17
 */
/* Overview:
 * For this laboratory assignment we expanded the blinking LED functionality of Lab 1 and
 * made the Atmel AVR Xplained Mini XMEGA328PB board interpret a string into Morse code 
 * using the LED. The instructions indicated that main() should call a child function, 
 * passing it a string, and that child function would blink the board's LED to represent 
 * the characters of the string. The string was our full name followed by RedID.  Helper
 * functions were used for the specific dot, dash, and space characters that make up Morse
 * code. The program ran on an endless loop with a significant pause between sessions to 
 * show the end of one and the beginning of the next. The program works as intended. 
 * Coupled with the homework, this laboratory shows the purpose of the DDR register. 
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define F_CPU 16000000UL	//16MHz clock from the debug prox
#include <avr/io.h>
#include <util/delay.h>

//	First, define helper functions for the management of dot/dash/spaces 
void dot(void){
	PORTB |= (1<<PORTB5);
	_delay_ms(200);
	PORTB &= (0<<PORTB5);
	_delay_ms(200);
}
void dash(void){
	PORTB |= (1<<PORTB5);
	_delay_ms(600);
	PORTB &= (0<<PORTB5);
	_delay_ms(200);
}
void spaceL(void){
	_delay_ms(400);
}
void spaceW(void){
	//each letter is off for 200ms + 400ms after letter, so 800ms left for space
	_delay_ms(800);
}

/*	Child function with massive switch statement that serves as dictionary for Morse Code interpreter
*/
void blinkMorse(char *n){
	
	int i;
	while(1){
		i=0;
		while(n[i]){
		
			switch(toupper(n[i])){
				case 'A':
					dot();
					dash();
					spaceL();
					break;
				case 'B':
					dash();
					dot();
					dot();
					dot();
					spaceL();
					break;
				case 'C':
					dash();
					dot();
					dash();
					dot();
					spaceL();
					break;
				case 'D':
					dash();
					dot();
					dot();
					spaceL();
					break;
				case 'E':
					dot();
					spaceL();
					break;
				case 'F':
					dot();
					dot();
					dash();
					dot();
					spaceL();
					break;
				case 'G':
					dash();
					dash();
					dot();
					spaceL();
					break;
				case 'H':
					dot();
					dot();
					dot();
					dot();
					spaceL();
					break;
				case 'I':
					dot();
					dot();
					spaceL();
					break;
				case 'J':
					dot();
					dash();
					dash();
					dash();
					spaceL();
					break;
				case 'K':
					dash();
					dot();
					dash();
					spaceL();
					break;
				case 'L':
					dot();
					dash();
					dot();
					dot();
					spaceL();
					break;
				case 'M':
					dash();
					dash();
					spaceL();
					break;
				case 'N':
					dash();
					dot();
					spaceL();
					break;
				case 'O':
					dash();
					dash();
					dash();
					spaceL();
					break;
				case 'P':
					dot();
					dash();
					dash();
					dot();
					spaceL();
					break;
				case 'Q':
					dash();
					dash();
					dot();
					dash();
					spaceL();
					break;
				case 'R':
					dot();
					dash();
					dot();
					spaceL();
					break;
				case 'S':
					dot();
					dot();
					dot();
					spaceL();
					break;
				case 'T':
					dash();
					spaceL();
					break;
				case 'U':
					dot();
					dot();
					dash();
					spaceL();
					break;
				case 'V':
					dot();
					dot();
					dot();
					dash();
					spaceL();
					break;
				case 'W':
					dot();
					dash();
					dash();
					spaceL();
					break;
				case 'X':
					dash();
					dot();
					dot();
					dash();
					spaceL();
					break;
				case 'Y':
					dash();
					dot();
					dash();
					dash();
					spaceL();
				break;
					case 'Z':
					dash();
					dash();
					dot();
					dot();
					spaceL();
					break;
				case '0':
					dash();
					dash();
					dash();
					dash();
					dash();
					spaceL();
					break;
				case '1':
					dot();
					dash();
					dash();
					dash();
					dash();
					spaceL();
					break;
				case '2':
					dot();
					dot();
					dash();
					dash();
					dash();
					spaceL();
					break;
				case '3':
					dot();
					dot();
					dot();
					dash();
					dash();
					spaceL();
					break;
				case '4':
					dot();
					dot();
					dot();
					dot();
					dash();
					spaceL();
					break;
				case '5':
					dot();
					dot();
					dot();
					dot();
					dot();
					spaceL();
					break;
				case '6':
					dash();
					dot();
					dot();
					dot();
					dot();
					spaceL();
					break;
				case '7':
					dash();
					dash();
					dot();
					dot();
					dot();
					spaceL();
					break;
				case '8':
					dash();
					dash();
					dash();
					dot();
					dot();
					spaceL();
					break;
				case '9':
					dash();
					dash();
					dash();
					dash();
					dot();
					spaceL();
					break;
				case ' ':
					spaceW();
					break;
			}
			i++;
		}
		//Input finished. Turn off for 5 seconds to show we are done with the input before looping.
		_delay_ms(5000);
	}
}

int main (void)
{
	//Prepare port bit B5 for output
	DDRB |= (1<<DDB5);
	
	char *whoami = "Gary Williams 803162497";
	//char *t = "test";
	blinkMorse(whoami);
}
