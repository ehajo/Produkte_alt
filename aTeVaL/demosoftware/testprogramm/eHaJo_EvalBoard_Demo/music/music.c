/*
 * music.c
 *
 * Created: 22.09.2013 19:09:07
 *  Author: calm
 */ 

#include "music.h"
#include "../gpio/gpio.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


ISR(TIMER1_COMPB_vect)
{
	// Summer-Pin toggeln
	PORTD ^= (1<<Summer);
}


const uint8_t melodie[]  = {
	14,	// Sand-
	14,	// mann
	11,	// lie-
	12,	// ber
	14,	// Sand-
	14,	// mann
	11,	// es
	11,	// ist
	12,	// noch
	9,	// nicht
	11,	// so
	7,	// weit!
	7,	// Wir
	9,	// sen-
	11,	// den
	9,	// erst
	11,	// den
	7,	// A-
	4,	// bend-
	6,	// ...
	7,	// gruß,
	9,	// eh
	7,	// ...
	6,	// je-
	9,	// des
	7,	// Kind
	9,	// ins
	11,	// Bett-
	12,	// chen
	14,	// muss.
	11,	// Du
	12,	// ...
	9,	// hast
	7,	// ge-
	9,	// wiss
	4,	// noch
	6,	// ...
	7	// Zeit.
};

// Die Dauer der Noten definieren, 1 = 1/8-Note, 2 = 1/4 usw.
const uint8_t dauer[38] = {2,2,1,1,2,2,2,3,1,1,1,4,2,3,1,1,1,2,1,1,4,1,1,1,1,1,1,1,1,4,1,1,1,1,2,1,1,6};



const unsigned int tonleiter[24]  = {
	TON_C,		//  0
	TON_CIS,	//  1
	TON_D,		//  2
	TON_ES,		//  3
	TON_E,		//  4
	TON_F,		//  5
	TON_FIS,	//  6
	TON_G,		//  7
	TON_GIS,	//  8
	TON_A,		//  9
	TON_B,		// 10
	TON_H,		// 11
	TON_CC,		// 12
	TON_CCIS,	// 13
	TON_DD,		// 14
	TON_EES,	// 15
	TON_EE,		// 16
	TON_FF,		// 17
	TON_FFIS,	// 18
	TON_GG,		// 19
	TON_GGIS,	// 20
	TON_AA,		// 21
	TON_BB,		// 22
	TON_HH		// 23
};



void play_tone()
{
	for(uint16_t i=0;i<1000;i++)	
	{
		PORTD ^= (1<<Summer);
		_delay_ms(2);
	}
}

void play_melody()
{
	/*
	// Laufvariable
	uint8_t i;
	
	// Timer1 ohne Vorteiler starten, CTC-Mode
	TCCR1B |= (1<<CS10) | (1<<WGM12);
	
	// Outputcompare-Register setzen, hier fuer ~440Hz
	OCR1B = TON_C;
	
	// Interrupt einschalten
	TIMSK |= (1<<OCIE1B);
	
	// Globales Interruptflag einschalten:
	sei();
	
	for(i = 0; i<38; i++)
	{	// Fuer alle Noten im Array
		
		// Compare-Wert entsprechend Array setzen
		OCR1B = tonleiter[melodie[i]];
		
		// Warten damit die Note klingen kann...
		long_delay(dauer[i]*300); // *300 klingt ganz gut bei dem Lied
		
		// Timer kurz aus- und wieder einschalten damit gleiche Noten
		// hintereinander unterschieden werden koennen
		TCCR1B = 0;
		long_delay(2);
		TCCR1B |= (1<<CS10) | (1<<WGM12);
	}
	
	// Lied Ende, Timer ausschalten:
	TCCR1B = 0;
	*/
	//ToDo: Timer umstellen
}