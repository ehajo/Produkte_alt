/*
 * LED-Blinker
 * Einfacher Code, der die LEDs abwechselnd blinken laesst
 * http://www.eHaJo.de
 * (C) Hannes Jochriem, 2013
 * 
 * µC-Belegung am Beispiel Atmega8:
 * LED1:			PD5
 * LED2: 		PD6
 */

#define F_CPU 8000000UL     /* 8MHz */

#include <avr/io.h>
#include <util/delay.h>

// Prototypen
void long_delay(uint16_t);

// Defines:
#define LED1		(1<<PD5)
#define LED2		(1<<PD6)

void main()
{
	uint16_t adc_wert = 0;
	
	// LED-Pins als Ausgang definieren
	DDRD |= LED1 | LED2;
	
	// LED1 einschalten
	PORTD |= LED1;
	
	while(1)
	{
		// LED1 und LED2 invertieren
		PORTD ^= LED1 | LED2;
		
		// 500ms lang warten
		long_delay(500);
	}
}

void long_delay(uint16_t ms) 
{
        for(; ms>0; ms--) 
                _delay_ms(1);
}
