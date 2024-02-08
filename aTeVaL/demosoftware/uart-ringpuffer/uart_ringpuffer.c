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
void long_delay(uint16_t);

// Defines:
#define LED1		(1<<PD6)
#define 	PUFFERSIZE	8

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

int main()
{
	// LED-Pin als Ausgang definieren
	DDRD |= LED1;
	
	// Receive-Interrupt einschalten, RX und TX einschalten
	UCSRB |= (1<<RXCIE) | (1<<RXEN) | (1<<TXEN);
	
	// 8 Bit Daten:
	UCSRC |= (1<<UCSZ0) | (1<<UCSZ1);
	
	// Baudrate auf 9600Bd einstellen:
	UBRRH = 0;
	UBRRL = 51; 
	
	// Globales Interruptflag einschalten:
	sei();
	
	while(1)
	{
		if(ring_lese != ring_schreibe)
		{ // He, es ist was im Rinpguffer!!!
			
			// Leseposition inkrementieren
			ring_lese++;
			
			// Leseposition auf Puffergroesse beschraenken
			ring_lese %= PUFFERSIZE;
			
			// Byte im Puffer zurücksenden
			UDR = ringpuffer[ring_lese];
			
			// Eine Sekunden warten damit man Versuchsweise den 
			// Rinpuffer volllaufen lassen kann
			long_delay(1000);
		}
	}
	
	return 0;
}

void long_delay(uint16_t ms) 
{
        for(; ms>0; ms--) 
                _delay_ms(1);
}