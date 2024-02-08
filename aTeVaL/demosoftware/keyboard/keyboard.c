/*
 * LED-Blinker
 * Einfacher Code, der den UART initialisiert und die 
 * empfangenen Bytes in einen Rinpuffe schiebt
 * Sind Bytes im Ringpuffer werden diese zurueckgeschickt
 * Als Baudrate wird 9600Bd eingestellt
 * http://www.eHaJo.de
 * (C) Hannes Jochriem, 2013
 */

#define F_CPU 8000000UL     /* 8MHz */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Prototypen
void sende_zeichen(uint8_t);
void long_delay(uint16_t);

// Defines:
#define SUMMER			(1<<PD7)
#define LED1		(1<<PD6)
#define 	PUFFERSIZE		8

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
// volatile ist noetig da die Variablen in der ISR veraendert werden
volatile uint8_t ringpuffer[PUFFERSIZE];
volatile uint8_t ring_schreibe = 0; 
volatile uint8_t ring_lese = 0;

// Interrupt Serviceroutine für "Byte am USART empfangen"
ISR(USART_RXC_vect)
{
	uint8_t temp;
	
	// Empfangenes Byte in den Speicher schreiben
	temp = UDR;
	
	// Schreibeposition inkrementieren
	ring_schreibe++;
	
	//Schreibeposition auf Puffergroesse beschraenken
	ring_schreibe %= PUFFERSIZE;
	
	if(ring_schreibe != ring_lese)
	{	// Puffer ist nicht voll, Byte schreiben
		// LED aus
		PORTD &= ~LED1;
		
		ringpuffer[ring_schreibe] = temp;
	}
	else
	{	// Puffer ist voll, Schreibeposition wieder dekrementieren
		// LED an zeigt vollen Buffer an
		PORTD |= LED1;
		
		ring_schreibe--;
	}
}

ISR(TIMER1_COMPA_vect)
{
	// Summer-Pin toggeln
	PORTD ^= SUMMER;
}

int main()
{
	// LED-Pin als Ausgang definieren
	DDRD |= LED1 | SUMMER;
	
	// Receive-Interrupt einschalten, RX und TX einschalten
	UCSRB |= (1<<RXCIE) | (1<<RXEN) | (1<<TXEN);
	
	// 8 Bit Daten:
	UCSRC |= (1<<UCSZ0) | (1<<UCSZ1);
	
	// Baudrate auf 9600Bd einstellen:
	UBRRH = 0;
	UBRRL = 51; 
	
	// Timer Capture Compare Interrupt einschalten
	TIMSK |= (1<<OCIE1A);
	
	// OCR1A vorladen:
	OCR1A = TON_A;
	
	TCCR1B = 0;
	
	// Globales Interruptflag einschalten:
	sei();
	
		// Startverzoegerung
	long_delay(1000);
	
	ring_lese = 0;
	ring_schreibe = 0;
	
	while(1)
	{
		if(ring_lese != ring_schreibe)
		{ // He, es ist was im Rinpguffer!!!
			
			// Leseposition inkrementieren
			ring_lese++;
			
			// Leseposition auf Puffergroesse beschraenken
			ring_lese %= PUFFERSIZE;
			
			// Die Taste auswerten und entsprechend den Ton einstellen:
			switch(ringpuffer[ring_lese])
			{
				case 'a':
					OCR1A = TON_C;
					sende_zeichen('C');
					break;
				case 's':
					OCR1A = TON_D;
					sende_zeichen('D');
					break;
				case 'd':
					OCR1A = TON_E;
					sende_zeichen('E');
					break;
				case 'f':
					OCR1A = TON_F;
					sende_zeichen('F');
					break;
				case 'g':
					OCR1A = TON_G;
					sende_zeichen('G');
					break;
				case 'h':
					OCR1A = TON_A;
					sende_zeichen('A');
					break;
				case 'j':
					OCR1A = TON_H;
					sende_zeichen('H');
					break;
				case 'k':
					OCR1A = TON_CC;
					sende_zeichen('c');
					break;
				case 'w':
					OCR1A = TON_CIS;
					sende_zeichen('C');
					sende_zeichen('i');
					sende_zeichen('s');
					break;
				case 'e':
					OCR1A = TON_ES;
					sende_zeichen('E');
					sende_zeichen('s');
					break;
				case 't':
					OCR1A = TON_FIS;
					sende_zeichen('F');
					sende_zeichen('i');
					sende_zeichen('s');
					break;
				case 'z':
					OCR1A = TON_GIS;
					sende_zeichen('G');
					sende_zeichen('i');
					sende_zeichen('s');
					break;
				case 'u':
					OCR1A = TON_B;
					sende_zeichen('B');
					break;
				default:
					OCR1A = 1;
					break;
			}
			sende_zeichen(' ');
			
			// Timer einschalten
			TCCR1B |= (1<<CS10) | (1<<WGM12);
			
			// Den Ton 300ms lang halten
			long_delay(300);
		}
		else
		{	// Timer ausschalten
			TCCR1B = 0;
		}
	}
	
	return 0;
}

void sende_zeichen(uint8_t zeichen)
{
	while (!(UCSRA & (1 << UDRE)))
	{}		// Warte bis der USART bereit ist
	UDR = zeichen;
}

void long_delay(uint16_t ms) 
{
        for(; ms>0; ms--) 
                _delay_ms(1);
}