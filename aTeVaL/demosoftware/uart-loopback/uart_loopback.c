/*
 * LED-Blinker
 * Einfacher Code, der den UART initialisiert und die 
 * empfangen Bytes wieder zuruecksendet (Interruptbasiert)
 * Als Baudrate wird 9600Bd eingestellt
 * http://www.eHaJo.de
 * (C) Hannes Jochriem, 2013
 */

#define F_CPU 8000000UL     /* 8MHz */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Interrupt Serviceroutine für "Byte am USART empfangen"
ISR(USART_RXC_vect)
{
	uint8_t temp;
 
	// Empfangens Byte in den Speicher schreiben:
	temp = UDR;
 
	// gespeichertes Byte wieder senden
	UDR = temp;
}

int main()
{	
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
		UDR = 'a';
		// nichts machen, der Code findet in der ISR statt
	}
	
	return 0;
}
