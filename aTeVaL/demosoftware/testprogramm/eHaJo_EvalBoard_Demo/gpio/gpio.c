/*
 * gpio.c
 *
 * Created: 20.09.2013 14:53:27
 *  Author: calm
 */ 
#include "gpio.h"
#include <avr/io.h>

void GPIO_Init()
{
	//Einmal in den Tristate Zustand
	DDRD=0x00;
	PORTD=0x00;
	
	//Nun die Richtung Anpassen
	DDRD |=( (1<<LED2) | (1<<LED1)  | (1<<Summer) );
	
	//Pullup für die Taster einschalten
	PORTD |=( (1<<Taster1) | (1<<Taster2) | (1<<Taster3) );
	
	
}



void Set_Led1(uint8_t Zustand)
{
	if(Zustand==0)	
	{
		PORTD&=~(1<<LED1);
	}
	else
	{
		PORTD|=(1<<LED1);
	}
	
}

uint8_t Get_Led1(void)
{
	return ((PORTD&(1<<LED1))>>LED1);
}

void Set_Led2(uint8_t Zustand)
{
	if(Zustand==0)
	{
		PORTD&=~(1<<LED2);
	}
	else
	{
		PORTD|=(1<<LED2);
	}
	
}

uint8_t Get_Led2(void)
{
	return ((PORTD&(1<<LED2))>>LED2);
}


void Set_Summer(uint8_t Zustand)
{
	if(Zustand==0)
	{
		PORTD&=~(1<<Summer);
	}
	else
	{
		PORTD|=(1<<Summer);
	}
	
}

void Toggle_Summer()
{
	PORTD^(1<<Summer);
}

uint8_t Get_Taster1()
{
	if(((PIND & (1<<Taster1))>>Taster1)==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t Get_Taster2()
{
	if(((PIND & (1<<Taster2))>>Taster2)==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


uint8_t Get_Taster3()
{
	if(((PIND & (1<<Taster3))>>Taster3)==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


