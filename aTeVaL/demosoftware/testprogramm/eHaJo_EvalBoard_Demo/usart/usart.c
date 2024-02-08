#include <avr/io.h> 		//IO Map für den Mega8
#include <avr/interrupt.h> 	//Interrupts für den Mega8
#include <inttypes.h>  		//wie der Name schon sagt
#include <stdlib.h> 		//das übliche c gerumpel

#include "./usart.h"
//So einmal USART Routunen für die uController
//USART Daten Senden


#if BUFFERED_TX > 0
#include "./fifo.h"
uint8_t buffer0[BUF_SIZE0];
fifo_t tx0_fifo;

void usart_putc(unsigned char c) {

	if((UCSRB&(1<<(UDRIE)))==0)
	{
		UCSRB |= (1<<(UDRIE));
	}
	
	while(fifo_put(&tx0_fifo,c)==0)
	{
		UCSRB |= (1<<(UDRIE));
	}
}


ISR(USART_UDRE_vect)
{
	//Okay dann einmal das nächste Byte nachlegen
	int from_buffer=fifo_get_nowait(&tx0_fifo);
	
	if(from_buffer>-1)
	{
		UDR=(unsigned char)(from_buffer);
	}
	else
	{
		//Okay der Fifo ist leer also ISR aus
		UCSRB &= ~(1<<(UDRIE));
	}
}
#else

void usart_putc(unsigned char c) {
   // wait until UDR ready
  	while(!(UCSRA & (1 << UDRE)));
		UDR = c;    // send character
   
}

#endif

void(*fptr)(void);
void Set_RX_Hook(void* funkptr)
{
fptr=funkptr;

#ifdef UBRRL
UCSRB|=(1<<RXCIE);
#elif defined UBRR0L
UCSR0B|=(1<<RXCIE0);
#endif

}

void Delete_RX_Hook(void)
{
	#ifdef UBRRL
	UCSRB&=~(1<<RXCIE);
	#elif defined UBRR0L
	UCSR0B&=~(1<<RXCIE0);
	#endif
fptr=0;
}


ISR(USART_RXC_vect)
{
	if(fptr!=0)
	{
		(*fptr)();	
	}
	else
	{
		uint8_t temp=UDR;
	}
}

void usart_init(usartparam_t* Param) 
{

#if BUFFERED_TX > 0
fifo_init (&tx0_fifo, buffer0, BUF_SIZE0);
#endif


unsigned int br=Param->BaudrateRegister;

UBRRL=(unsigned char)br;
UBRRH=(unsigned char)(br>>8);
UCSRB =(1<<RXEN)|(1<<TXEN);
//Parity einstellen:
//Einmal die Bits löschen (default = None Parity) also einen definierten Zustand schaffen
//Transmittermode auf USART 
//Einmal einen definierten Zustand schaffen
//Einstellung laut Datenblatt Atmel
UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UCPOL); //Muss laut Datenblatt eins sein um das Register zu schreiben
//Setzt den Baustein in den Asynchronen Modus

uint8_t UCSRC_Temp=(1<<URSEL); //Die einstellungen werden in eine temporäre Variable geschrieben, das das UCSRC und UBRRH die gleiche Speicheradresse haben
//Mit dem Zug umgeht man das eine oder andere Problem :-(

switch(Param->Paritytype)
{
	case None:
	{
		UCSRC_Temp&=~( (1<<UPM0)||(1<<UPM1) );		
	}
	break;
	
	case Even:
	{
		UCSRC_Temp|=(1<<UPM1);		
	}
	break;
	
	case Odd:
	{
		UCSRC_Temp|=(1<<UPM0)||(1<<UPM1);
	}
	break;
	
	default:
	{
		UCSRC_Temp&=~( (1<<UPM0)||(1<<UPM1) );
	}
	break;
}

switch(Param->NoOfBits)
{
	case Nine:
	{
		UCSRC_Temp |=( (1<<UCSZ2) | (1<<UCSZ1) | (1<<UCSZ0));
	}
	
	case Eight:
	{
		UCSRC_Temp |=( (0<<UCSZ2) | (1<<UCSZ1) | (1<<UCSZ0));
	}
	break;
	
	case Seven:
	{
		UCSRC_Temp |=( (0<<UCSZ2) | (1<<UCSZ1) | (0<<UCSZ0));
	}
	break;
	
	case Six:
	{
		UCSRC_Temp |=( 0<<UCSZ2) | (0<<UCSZ1) | (1<<UCSZ0);	
	}
	break;
	
	case Five:
	{
		UCSRC_Temp |=( (0<<UCSZ2) | (0<<UCSZ1) | (0<<UCSZ0));	
	}
	break;
	
	default:
	{
		UCSRC_Temp |=( (0<<UCSZ2) | (1<<UCSZ1) | (1<<UCSZ0));
	}
	break;
}

UCSRC=UCSRC_Temp;

if(Param->Doublespeed>0)
{
	UCSRA |= (1<<U2X); //Die Baudrate Verdoppeln auf 9600 Baud
}
else
{
	UCSRA &= ~(1<<U2X); //Die Baudrate Verdoppeln auf 9600 Baud
}

#if BUFFERED_TX > 0
UCSRB |= (1<<(UDRIE));
#endif

}
