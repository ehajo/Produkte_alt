/*
 * wuerfel-smd.c
 *
 * Created: 14.10.2019 10:25:11
 * Author : hannes
 * CC BY-NC-SA
 */ 

#define F_CPU 1200000UL     /* Internal 9,6MHz CKDIV8 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdlib.h>

void long_delay(uint16_t);
void wuerfel(uint8_t);
volatile uint16_t Timer = 0;
volatile uint8_t AugenTimer = 0;

int main()
{
	// Standard values
	uint8_t i;
	uint16_t verzoegerung = 0;
	uint8_t auge = 7;
	uint8_t auge2 = 7;

	// Set LED-Pins as Output, pullup for Switch
	DDRB = 0b00011101;
	PORTB |= (1<<PB1);

	// Configure timer and INT
	TCCR0B |=  (1<<CS00);
	TIMSK0 |= (1<<TOIE0);

	sei();	// enable interrupt

	while (1)
	{	// main loop
		if(Timer>20000l)
		{	// if 2s are over: go to sleep
			wuerfel(7);	// turn off all LEDs
			GIMSK |= (1 << INT0);	// activate interrupt
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);	// sleep mode
			sleep_mode()	// zzZZZ
			GIMSK &= ~(1 << INT0);	// disable interrupt
		}
		if ( !(PINB & (1<<PINB1)) )
		{	// key pressed
			verzoegerung = 0;
			long_delay(200);
			if ( !(PINB & (1<<PINB1)) )
			{	// calculate and animate while button pressed
				auge = TCNT0;
				auge2 = auge%6;
				while(!(PINB & (1<<PINB1)))
				{
					for(i=0;i<7;i++)
					{
						auge += i;
						auge%=6;
						wuerfel(auge);
						long_delay(20);
					}
				}
				for(i=0;i<18;i++)
				{
					long_delay(20+verzoegerung);
					wuerfel((i)%6);
					verzoegerung+=(i);
				}
				long_delay(verzoegerung);
				wuerfel(auge2);
				long_delay(400);
				wuerfel(7);
				long_delay(200);
				wuerfel(auge2);
				Timer=0;
			}
			else
			{	// show value
				wuerfel(auge2);
				Timer=0;
			}
		}
	}

	return 0;
}

void wuerfel(uint8_t zahl)
{
	PORTB |= 0b00011101;
	switch (zahl)
	{
		case 0:	// wuerfel 1
		PORTB &= ~((1<<PB3));
		break;
		case 1:	// wuerfel 2
		PORTB &= ~((1<<PB4));
		break;
		case 2:	// wuerfel 3
		PORTB &= ~((1<<PB3) | (1<<PB4));
		break;
		case 3:	// wuerfel 4
		PORTB &= ~((1<<PB4) | (1<<PB2));
		break;
		case 4:	// wuerfel 5
		PORTB &= ~((1<<PB4) | (1<<PB2) | (1<<PB3));
		break;
		case 5:	// wuerfel 6
		PORTB &= ~((1<<PB4) | (1<<PB2) | (1<<PB0));
		break;
		default:
		break;
	}
}


void long_delay(uint16_t ms)
{
	for(; ms>0; ms--)
	_delay_ms(1);
}

ISR(TIM0_OVF_vect)
{
	Timer++;
}

ISR(INT0_vect) {
}


