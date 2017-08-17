/*	Gary Williams
 *	COMPE 375-01
 *	Lab 8 Due: 4/14/17
 *
 *  A/D Conversion:
 *  Used interrupts and ADC to control LED brightness
 *  according to analog result read from the
 *  potentiometer. ADC auto-trigger set to Timer0CompareA.
 *
 *  Timer0 is set to 4ms, the ADC uses channel ADC1, and 
 *  the PWM freq of the LED is 800HZ.
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
void AD_set_timer(short key);
volatile short potVar;

int main (void)
{
	short temp_comp = potVar;
	lab_init();
	while(1){
		if(temp_comp != potVar){
			AD_set_timer(potVar);
			temp_comp = potVar;
		}
	}
}

void lab_init(void){
	//PINB5 out for LED, B4 for 4ms waveform, B3 for AD ISR waveform
	DDRB |= (1<<DDB5|1<<DDB4|1<<DDB3);
	DDRC &= ~(1<<DDC1);			//PINC1 in for var input
	ADMUX |= (1<<REFS0|1<<MUX0);	//Vref external and select channel ADC1

	//Set up keyboard scanning timer for every 4ms
	TCCR0A |= (1<<WGM01);
	OCR0A = 0xF9;
	
	//clear global interrupt flag to prevent interrupt while setting up
	cli();
	TIMSK0 |= (1<<OCIE0A);	//Set interrupt on timer0 compareA
	//Now set for timer1 compareA and compareB
	TIMSK1 |= (1<<OCIE1A | 1<<OCIE1B);
	
	ADCSRA |= (1<<ADEN|1<<ADATE|1<<ADIE|1<<ADPS1|1<<ADPS0);
	ADCSRB |= (1<<ADTS1|1<<ADTS0);	//Autotrigger timer0compareA
	sei();
	
	//Turn on the timer for scanning the keyboard
	TCCR0B |= (1<<CS02);
}

void AD_set_timer(short key){
	//PWM frequency of 800Hz; that's a period of 1.25 ms
	OCR1A = 20000;		//figured by proportional to no prescale max for 16 bit reg
	OCR1B = (key/1024.) * 20000;
	TCCR1B |= (1<<WGM12 | 1<<CS10);		//CTC mode without prescale- max 4.096 sec
}

ISR(TIMER0_COMPA_vect){		//This is the ISR for the 4ms
	PINB ^= (1<<PINB4);		//Toggle PINB4 to show waveform
}

ISR(TIMER1_COMPA_vect){
	LED_ON;
}

ISR(TIMER1_COMPB_vect){
	LED_OFF;
}

ISR(ADC_vect){
	potVar = ADC;
	PINB ^= (1<<PINB3);		//Toggle PINB3 to show waveform
}
