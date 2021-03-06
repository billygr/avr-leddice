#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdlib.h>

/* Updated to work with the newer hardware revision     */

/* PORTD pin 2 is used for the button (INT0 pin)        */
#define BUTTON_PIN        2

#define SET_BIT(PORT_NR,BIT) (PORT_NR |= (1<<BIT))
#define CLEAR_BIT(PORT_NR,BIT) (PORT_NR &= ~(1<<BIT))

/* PORTB and PORTD pins are used for the LEDs
 * Define Pins for the LEDs
 */

/* Left LEDs    */
#define LEFT_LED4       PD6
#define LEFT_LED2LED6   PD5
#define LEFT_LED1LED7   PD4
#define LEFT_LED3LED5   PD3

/* Right LEDs   */
#define RIGHT_LED4      PB3
#define RIGHT_LED2LED6  PB2
#define RIGHT_LED1LED7  PB1
#define RIGHT_LED3LED5  PB0

/* Numbers on the LEDs
 * 1 => 4
 * 2 => 2 6
 * 3 => 2 4 6
 * 4 => 1 2 6 7
 * 5 => 1 2 6 7 4
 * 6 => 1 2 3 5 6 7
 */

/*SET_BIT(PORTD,3);_delay_ms(500);PORTD = 0x00;
SET_BIT(PORTD,4);_delay_ms(500);PORTD = 0x00;
SET_BIT(PORTD,3);SET_BIT(PORTD,4);_delay_ms(500);PORTD = 0x00;
SET_BIT(PORTD,5);SET_BIT(PORTD,4);_delay_ms(500);PORTD = 0x00;
SET_BIT(PORTD,5);SET_BIT(PORTD,4);SET_BIT(PORTD,3);_delay_ms(500);PORTD = 0x00;
SET_BIT(PORTD,6);SET_BIT(PORTD,5);SET_BIT(PORTD,4);_delay_ms(500);PORTD = 0x00;
*/

const int L_leds[6] = { 8, 16, 24, 48, 56, 112};
const int R_leds[6] = { 8, 4, 12, 6, 14, 7};

ISR(INT0_vect)
{
        /* Generate a random value for the LEDs
         * and display the value for 3 seconds
         */
        srand(TCNT1);
        PORTD = PORTD | L_leds[rand() % 6];
        PORTB = PORTB | R_leds[rand() % 6];
        _delay_ms(3000);
        PORTB=0x00;
        PORTD=0x00;

        // Make the INTO high to avoid triggering again
        SET_BIT(PORTD,BUTTON_PIN);
        /* Debounce     */
        //loop_until_bit_is_clear(PIND,BUTTON_PIN);
}

int main (void)
{
        int i=0;

        /* PORTB and PORTD are used for output     */
        DDRB = 0xFF;
        DDRD = 0xFF;

        /* All port pins to Low Level = LEDs OFF       */
        PORTB = 0x00;
        PORTD = 0x00;

        /* Setup a timer running. We will use it for random seed        */
        TCCR1B |= (1 << CS10);

        /* Self Test
         * counts from 1 to 6 on the dice with 500 ms delay then switches
         * off the LEDs
         */

        for (i=0;i<=5;i++)
        {
                PORTD = PORTD | L_leds[i];
                _delay_ms(500);
                PORTD=0x00;
        }

        /* All port pins to Low Level = LEDs OFF       */
        PORTB = 0x00;
        PORTD = 0x00;

        DDRD &= ~(1 << DDD2);       // Clear the PD2 pin (INT0)
        PORTD |= (1 << PORTD2);     // Turn on the internal pull-up

        /* Interrupt on INT0 (button) on low level
         * Only a low level can wake up the MCU from sleep
        */
        MCUCR =(0<<ISC01) | (0<<ISC00);

        /* Turn on interrupts   */
        GIMSK  |= (1<<INT0);

        /* Global enable the interrupts */
        sei ();

        /*      Main program loop       */
        while (1)
        {
                set_sleep_mode(SLEEP_MODE_PWR_DOWN);
                sleep_enable();
                sleep_cpu();
                sleep_disable();
        }
}
