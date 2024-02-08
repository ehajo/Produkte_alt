/*
 * Tonausgabe
 * Code, um einen Ton mit Hilfe eines Timers zu erzeugen
 * http://www.eHaJo.de
 * (C) Hannes Jochriem, 2013
 */

#define F_CPU 8000000UL     /* 8MHz */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Defines:
#define SUMMER			(1<<PD7)
#define TASTE1		(1<<PD2)
#define TASTE2		(1<<PD3)

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

ISR(TIMER1_COMPA_vect)
{
	// Summer-Pin toggeln
	PORTD ^= SUMMER;
}

int main()
{	
	// Summer als Ausgang definieren
	DDRD |= SUMMER;
	
	// Pullups der Tasten einschalten
	PORTD |= TASTE1 | TASTE2;
	
	// Outputcompare-Register setzen, hier fuer Ton A
	// Um einen anderen Ton auszugeben muss man hier
	// einen anderen Wert aus den Defines eintragen
	OCR1A = TON_A;
	
	// Interrupt einschalten
	TIMSK |= (1<<OCIE1A);
	
	// Globales Interruptflag einschalten:
	sei();
	
	while(1)
	{
		if(!(PIND & TASTE1))
		{	// Taste 1 wurde gedrueckt, Ton starten
			// Timer1 ohne Vorteiler starten, CTC-Mode
			TCCR1B |= (1<<CS10) | (1<<WGM12);
		}
		if(!(PIND & TASTE2))
		{	// Taste 2 gedrueckt, Ton aus
			// Timer1 stoppen
			TCCR1B = 0;
		}
	}
	
	return 0;
}