/*
 * adc.c
 *
 * Created: 20.09.2013 19:29:13
 *  Author: calm
 */ 
#include "adc.h"

void adc_init()
{
	DDRC &= ~((1<<PC1)|(1<<PC0));
	PORTC &= ~((1<<PC1)|(1<<PC0));
	
	// AVCC (also +5V als Spannungsreferenz verwenden)
	ADMUX |= (1<<REFS0);
	
	// Teilungsfaktor des ADC, ADC-Frequenz soll zw. 50 und 200kHz liegen.
	// In diesem Fall Teiler 64, also dann 1MHz / 8 = 125kHz
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0);
	
	// ADC einschalten
	ADCSRA |= (1<<ADEN);
	
	// Nach dem Einschalten wird eine Dummy-Wandlung empfohlen
	// Wandlung starten
	ADCSRA |= (1<<ADSC);
	
	// Warten bis die Wandlung fertig ist.
	while (ADCSRA & (1<<ADSC))
	{}
	
	// Ergebnis der Wandlung auslesen
	//uint8_t adcwert = ADCW;
	
}

uint16_t Get_ADC_Value(uint8_t channel)
{
	channel&=0x0F;
	ADMUX&=~(0x0F); 
	ADMUX|=channel;
	
	// Wandlung starten
	ADCSRA |= (1<<ADSC);
	
	// Warten bis die Wandlung fertig ist.
	while (ADCSRA & (1<<ADSC))
	{}
	
	// Ergebnis der Wandlung auslesen
	return ADCW;
}
