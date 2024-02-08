/*
 * FreeModbus Libary: ATMega168 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *   - Initial version and ATmega168 support
 * Modfications Copyright (C) 2006 Tran Minh Hoang:
 *   - ATmega8, ATmega16, ATmega32 support
 *   - RS485 support for DS75176
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.6 2006/09/17 16:45:53 wolti Exp $
 */

#include <avr/io.h>
#include <avr/interrupt.h>


#include "port.h"

#include "../../usart/usart.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "../include/mb.h"
#include "../include/mbport.h"

#define UART_BAUD_RATE          9600
#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) \
( ( F_OSC ) / ( ( UART_BAUD_RATE ) * 16UL ) - 1 )

//#define UART_UCSRB  UCSR0B



void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	#ifdef RTS_ENABLE
	UCSRB |= _BV( TXEN ) | _BV(TXCIE);
	#else
	UCSRB |= _BV( TXEN );
	#endif

	if( xRxEnable )
	{
		UCSRB |= _BV( RXEN ) | _BV( RXCIE );
	}
	else
	{
		UCSRB &= ~( _BV( RXEN ) | _BV( RXCIE ) );
	}

	if( xTxEnable )
	{
		UCSRB |= _BV( TXEN ) | _BV( UDRE );
		#ifdef RTS_ENABLE
		RTS_HIGH;
		#endif
	}
	else
	{
		UCSRB &= ~( _BV( UDRE ) );
	}
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
   
	//UCHAR ucUCSRC = 0;
   
	/* prevent compiler warning. */
	
	(void)ucPORT;
	
	
	//UBRR = UART_BAUD_CALC( ulBaudRate, F_CPU );
	usartparam_t Param;
	Param.BaudrateRegister=UART_BAUD_CALC( ulBaudRate, F_CPU );
	Param.Doublespeed=0;
	Param.NoOfBits=One;

	switch ( eParity )
	{
		case MB_PAR_EVEN:
		//ucUCSRC |= _BV( UPM1 );
		Param.Paritytype=Even;
		break;
		case MB_PAR_ODD:
		//ucUCSRC |= _BV( UPM1 ) | _BV( UPM0 );
		Param.Paritytype=Odd;
		break;
		case MB_PAR_NONE:
		Param.Paritytype=None;
		break;
	}

	switch ( ucDataBits )
	{
		case 8:
		//ucUCSRC |= _BV( UCSZ0 ) | _BV( UCSZ1 );
		Param.NoOfBits=Eight;
		break;
		case 7:
		//ucUCSRC |= _BV( UCSZ1 );
		Param.NoOfBits=Seven;
		break;
	}
/*
	#if defined (__AVR_ATmega168__)
	UCSRC |= ucUCSRC;
	#elif defined (__AVR_ATmega169__)
	UCSRC |= ucUCSRC;
	#elif defined (__AVR_ATmega8__)
	UCSRC = _BV( URSEL ) | ucUCSRC;
	#elif defined (__AVR_ATmega16__)
	UCSRC = _BV( URSEL ) | ucUCSRC;
	#elif defined (__AVR_ATmega32__)
	UCSRC = _BV( URSEL ) | ucUCSRC;
	#elif defined (__AVR_ATmega128__)
	UCSRC |= ucUCSRC;
	#endif
    */

	
	
	
	usart_init(&Param);
	Delete_RX_Hook();
	Set_RX_Hook(pxMBFrameCBByteReceived);
	vMBPortSerialEnable( FALSE, FALSE );

	#ifdef RTS_ENABLE
	RTS_INIT;
	#endif
	return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	//UDR = ucByte;
	usart_putc(ucByte);
	return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
	
	*pucByte = UDR;
	return TRUE;
}

ISR( USART_UDRE_vect )
{
	pxMBFrameCBTransmitterEmpty(  );
}

/*
SIGNAL( SIG_USART_RECV )
{
	pxMBFrameCBByteReceived(  );
}
*/

#ifdef RTS_ENABLE
SIGNAL( SIG_UART_TRANS )
{
	RTS_LOW;
}
#endif

