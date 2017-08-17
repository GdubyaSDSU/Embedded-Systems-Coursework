/*	Gary Williams
 *	COMPE 375-01
 *	Lab 7A Due: 3/24/17
 *
 *  Interrupts:
 *		Used two interrupts; one timer and one
 *  pin change. The pin change is toggled by the
 *  push button and turns the LED on. The timer
 *  interrupt scans a row of a keyboard. If a key
 *  is pressed, a global var is changed which main
 *  then sees and sets the timers compare values
 *  before starting timer1.
 */
#define F_CPU 16000000UL
#define BAUDRATE ((F_CPU)/(ubrr*16UL)-1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#define LED_ON PORTB |= (1<<PORTB5);
#define LED_OFF PORTB &= ~(1<<PORTB5);

void lab_init(void);
void keyboard_init(void);
void keypad_set_timer(short key);
void keypad_scan(short i); 
volatile short keypad_var = 0;
volatile short keypad_row = 0;

int main (void)
{
	short temp_comp = keypad_var;
	lab_init();
	while(1){
		if(temp_comp != keypad_var){
			if(keypad_var >= 0){
				keypad_set_timer(keypad_var);
			}
			temp_comp = keypad_var;
		}
	}
}

void lab_init(void){
	keyboard_init();
	//Set up keyboard scanning timer for every 4ms
	TCCR0A |= (1<<WGM01);
	OCR0A = 0xF9;
	//clear global interrupt flag to prevent interrupt while setting up
	cli();
	TIMSK0 |= (1<<OCIE0A);
	//Just set interrupt for timer0 compare, now set for timer1 compare
	TIMSK1 |= (1<<OCIE1A | 1<<OCIE1B);
	
	//Turn on pin change interrupt in Pin Change Interrupt Control Register
	PCICR |= (1<<PCIE0);
	//Turn on specific pin change interrupts in Pin Change Mask Register
	//PCMSK0 |= (1<<PCINT0 | 1<<PCINT1 | 1<<PCINT2 | 1<<PCINT3 | 1<<PCINT7);
	PCMSK0 |= (1<<PCINT7);
	sei();
	//Turn on the timer for scanning the keyboard
	TCCR0B |= (1<<CS02);
}

void keyboard_init(void){
	DDRB = 0x20;	//PinB5 out and we want PinB7 in
	DDRC = 0x01;	//PinC0 out
	DDRD = 0xF0;	//PinD4-D7 out
	PORTB = 0x0F;	//Make PINB0-B3 pull-up
	PORTD = 0xFF;	//Make PIND4-D7 ON
}

void keypad_set_timer(short key){
	//PWM frequency of 800Hz; that's a period of 1.25 ms
	OCR1A = 20000;		//figured by proportional to no prescale max for 16 bit reg
	OCR1B = key * 2000;
	TCCR1B |= (1<<WGM12 | 1<<CS10);		//CTC mode without prescale- max 4.096 sec
}

void keypad_scan(short i){
	short j;
	static unsigned char row[] = {0xEF, 0xDF, 0xBF, 0x7F};
	static short keypad[4][4] = {{1, 2, 3, -1}, {4, 5, 6, -1}, {7, 8, 9, -1}, {-1, 0, -1, -1}};
	PORTD = row[i];
	for (j=0; j<4; j++){
		if(!(PINB & (1<<j))){
			_delay_ms(10);
			if(!(PINB & (1<<j))){
				keypad_var = keypad[i][j];
			}
		}
	}
}

ISR(PCINT0_vect){
	LED_ON;
	while(!(PINB & (1<<PINB7)));	//Check to see button is still being held
	LED_OFF;
	//Wanted to put in something to clear Timer1
	//For some reason, actually needs to be pressed twice
	keypad_var = 0;
	TCCR1B &= 0xF8;
}

ISR(TIMER0_COMPA_vect){		//This is the ISR for the 4ms keyboard scan
	keypad_row = (keypad_row+1) % 4;
	PORTC ^= 0x01;
	keypad_scan(keypad_row);
}

ISR(TIMER1_COMPA_vect){
	LED_ON;
}

ISR(TIMER1_COMPB_vect){
	LED_OFF;
}