#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdlib.h>

/* PORTD pin 2 is used for the button (INT0 pin)	*/
#define BUTTON_BIT        2

/* PORTB pins are used for the LEDs
 * Define Pins for the LEDs
 */

/* Left LEDs	*/
#define LEFT_LED4	PB7
#define LEFT_LED2LED6	PB6
#define	LEFT_LED1LED7	PB5
#define	LEFT_LED3LED5	PB4

/* Right LEDs	*/
#define RIGHT_LED4	PB3
#define RIGHT_LED2LED6	PB2
#define	RIGHT_LED1LED7	PB1
#define	RIGHT_LED3LED5	PB0

/* Numbers on the LEDs
 * 1 => 4
 * 2 => 2 6
 * 3 => 2 4 6
 * 4 => 1 2 6 7
 * 5 => 1 2 6 7 4
 * 6 => 1 2 3 5 6 7
 */

/* The ideal working code 
const int left_leds[6] = {~(_BV(LEFT_LED4)), 
			  ~(_BV(LEFT_LED2LED6)),
			  ~(_BV(LEFT_LED2LED6))+~(_BV(LEFT_LED4)),
			  ~(_BV(LEFT_LED2LED6))+~(_BV(LEFT_LED1LED7)),
			  ~(_BV(LEFT_LED2LED6))+~(_BV(LEFT_LED1LED7))+~(_BV(LEFT_LED4)),
			  ~(_BV(LEFT_LED2LED6))+~(_BV(LEFT_LED1LED7))+~(_BV(LEFT_LED3LED5))};

const int right_leds[6] = {~(_BV(RIGHT_LED4)), 
			  ~(_BV(RIGHT_LED2LED6)),
			  ~(_BV(RIGHT_LED2LED6))+~(_BV(RIGHT_LED4)),
			  ~(_BV(RIGHT_LED2LED6))+~(_BV(RIGHT_LED1LED7)),
			  ~(_BV(RIGHT_LED2LED6))+~(_BV(RIGHT_LED1LED7))+~(_BV(RIGHT_LED4)),
			  ~(_BV(RIGHT_LED2LED6))+~(_BV(RIGHT_LED1LED7))+~(_BV(RIGHT_LED3LED5))};
*/

/* The working code :)	*/
const int L_leds[6] = { 8, 4, 12, 6, 14, 7};
const int R_leds[6] = { 8, 4, 12, 6, 14, 7};

static int dice=0;

ISR(INT0_vect)
{
	/* Generate a random value for the LEDs
	 * and display the value for 5 seconds
	 */
	srand(TCNT1);
	dice = L_leds[rand() % 6] * 0x10 + R_leds[rand() % 6];
	PORTB = ~dice;
	_delay_ms(5000);
	PORTB=0xFF;
		
	/* Debounce	*/
	loop_until_bit_is_set(PIND,BUTTON_BIT);
}

int main (void)
{
	int i=0;

	/* PortB is used for output	*/
	DDRB = 0xFF;

	/* All port pins to High Level = LEDs OFF	*/
	PORTB = 0xFF;

        /* PortD is used for input      */
        DDRD = 0x00;

	/* PortD enable the pull ups	*/
	PORTD = 0xFF;

	/* Setup a timer running. We will use it for random seed	*/
	TCCR1B |= (1 << CS10);

	/* Self Test
	 * counts from 1 to 6 on the dice with 500 ms delay then switches
	 * off the LEDs
	 */

	for (i=0;i<=5;i++)
	{
		dice = L_leds[i]*0x10+R_leds[i];
		PORTB = ~dice;
		_delay_ms(500);
	}

	/* All port pins to High Level = LEDs OFF	*/
	PORTB = 0xFF;

	/* interrupt on INT0 (button) on low level	*/
	MCUCR = (0<<ISC01) | (0<<ISC00);

	/* Turn on interrupts	*/
	GIMSK  |= (1<<INT0);

	/* Global enable the interrupts	*/
	sei ();

	/*	Main program loop	*/
	while (1)
	{
//		set_sleep_mode((_BV(SM0) | _BV(SM1)));
//		sleep_enable();
//		sleep_mode();
//       	sei();
	}
}
