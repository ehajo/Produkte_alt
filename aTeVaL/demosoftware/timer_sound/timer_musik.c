/*
 * Liederausgabe aufbauend auf dem 
 * Democode "Tonausgabe mittels Timer"
 * 
 * Code, um eine Tonfolge mit definierter Laenge auszugeben
 * http://www.eHaJo.de
 * (C) Hannes Jochriem, 2013
 */

#define F_CPU 8000000UL     /* 8MHz */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Prototypen
void long_delay(uint16_t);

// Defines:
#define SUMMER			(1<<PD7)

// Diese Werte berechnen sich aus "8MHz / Frequenz des Tones / 2"
// Sollte ein anderer CPU-Takt verwendet werden muessen
// die Werte neu berechnet werden.
#define TON_C			15152l
#define TON_CIS		14545l
#define TON_D			13468l
#define TON_ES			12618l
#define TON_E			12121l
#define TON_F			11364l
#define TON_FIS		10899l
#define TON_G			10101l
#define TON_GIS		9479l
#define TON_A			9091l
#define TON_B			8421l
#define TON_H			8081l
#define TON_CC			7576l
#define TON_CCIS		7273l
#define TON_DD			6734l
#define TON_EES		6309l
#define TON_EE			6061l
#define TON_FF			5682l
#define TON_FFIS		5450l
#define TON_GG			5051l
#define TON_GGIS		4740l
#define TON_AA			4546l
#define TON_BB			4211l
#define TON_HH			4041l

// globale Variablen

// In diesem Array stehen die Werte fuer den CTC-Counter
uint16_t tonleiter[24] = {
	TON_C,			//  0
	TON_CIS,		//  1
	TON_D,			//  2
	TON_ES,		//  3
	TON_E,			//  4
	TON_F,			//  5
	TON_FIS,		//  6
	TON_G,			//  7
	TON_GIS,		//  8
	TON_A,			//  9
	TON_B,			// 10
	TON_H,			// 11
	TON_CC,		// 12
	TON_CCIS,	// 13
	TON_DD,		// 14
	TON_EES,		// 15
	TON_EE,		// 16
	TON_FF,		// 17
	TON_FFIS,	// 18
	TON_GG,		// 19
	TON_GGIS,	// 20
	TON_AA,		// 21
	TON_BB,		// 22
	TON_HH			// 23
};

// Melodie definieren, in diesem Fall 
uint8_t melodie[38] = {
	14,	// Sand-
	14,	// mann
	11,	// lie-
	12,	// ber
	14,	// Sand-
	14,	// mann
	11,	// es
	11,	// ist
	12,	// noch
	9,		// nicht
	11,	// so
	7,		// weit!
	7,		// Wir
	9,		// sen-
	11,	// den
	9,		// erst
	11,	// den
	7,		// A-
	4,		// bend-
	6,		// ...
	7,		// gruß,
	9,		// eh
	7,		// ...
	6,		// je-
	9,		// des
	7,		// Kind
	9,		// ins
	11,	// Bett-
	12,	// chen
	14,	// muss.
	11,	// Du
	12,	// ...
	9,		// hast
	7,		// ge-
	9,		// wiss
	4,		// noch
	6,		// ...
	7		// Zeit.
};

// Die Dauer der Noten definieren, 1 = 1/8-Note, 2 = 1/4 usw.
uint8_t dauer[38] = {2,2,1,1,2,2,2,3,1,1,1,4,2,3,1,1,1,2,1,1,4,1,1,1,1,1,1,1,1,4,1,1,1,1,2,1,1,6};

ISR(TIMER1_COMPA_vect)
{
	// Summer-Pin toggeln
	PORTD ^= SUMMER;
}

int main()
{
	// Laufvariable
	uint8_t i;
	
	// Summer als Ausgang definieren
	DDRD |= SUMMER;
	
	// Timer1 ohne Vorteiler starten, CTC-Mode
	TCCR1B |= (1<<CS10) | (1<<WGM12);
	
	// Outputcompare-Register setzen, hier fuer ~440Hz
	OCR1A = TON_C;
	
	// Interrupt einschalten
	TIMSK |= (1<<OCIE1A);
	
	// Globales Interruptflag einschalten:
	sei();
	
	for(i = 0; i<38; i++)
	{	// Fuer alle Noten im Array
	
		// Compare-Wert entsprechend Array setzen
		OCR1A = tonleiter[melodie[i]];
		
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
	
	while(1)
	{
		// nix...
	}
	
	return 0;
}


void long_delay(uint16_t ms) 
{
	for(; ms>0; ms--) 
		_delay_ms(1);
}