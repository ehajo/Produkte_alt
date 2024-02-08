/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            15

#define digitalPinHasPWM(p)         ((p) == 4 || (p) == 5 || (p) == 6 || (p) == 15)

static const uint8_t SS   = 6;
static const uint8_t MOSI = 7;
static const uint8_t MISO = 8;
static const uint8_t SCK  = 9;
static const uint8_t CS1  = 2;
static const uint8_t CS2  = 3;
static const uint8_t CS3  = 4;
static const uint8_t CS4  = 5;

// ATMEL ATTiny 2313
//
//                  +-\/-+
//      (RST) PA2  1|    |28  VCC
//      (D 0) PD0  2|    |27  PB7 (D 13)
//      (D 1) PD1  3|    |26  PB6 (D 12)
//            PA1  4|    |25  PB5 (D 11)
//            PA0  5|    |24  PB4 (D 10)
//      (D 2) PD2  6|    |23  PB3 (D  9)
//      (D 3) PD3  7|    |22  PB2 (AI 0)
//      (D 4) PD4  8|    |21  PB1 (D  7)
//      (D 5) PD5  9|    |20  PB0 (D  8)
//            GND 10|    |19  PD6 (D  6)
//                  +----+


#ifdef ARDUINO_MAIN

const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	NOT_A_PORT,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	NOT_A_PORT,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	NOT_A_PORT,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PB,	// 2	PB0
	PB,	// 3	PB1
	PB,	// 4	PB2
	PB,	// 5	PB3
	PB,	// 6	PB4
	PB,	// 7	PB5
	PB,	// 8	PB6
	PB,	// 9	PB7
	PD,	// 10	PD0
	PD,	// 11	PD1
	PD,	// 12	PD2
	PD,	// 13	PD3
	PD,	// 14	PD4
	PD,	// 15	PD5
	PD,	// 16	PD6
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0),	// PB0
	_BV(1),	// PB1
	_BV(2),	// PB2
	_BV(3),	// PB3
	_BV(4),	// PB4
	_BV(5),	// PB5
	_BV(6),	// PB6
	_BV(7),	// PB7
	_BV(0),	// PD0
	_BV(1),	// PD1
	_BV(2),	// PD2
	_BV(3),	// PD3
	_BV(4),	// PD4
	_BV(5),	// PD5
	_BV(6),	// PD6
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,	// PB0
	NOT_ON_TIMER,	// PB1
	TIMER0A,			// PB2
	TIMER1A,			// PB3
	TIMER1B,			// PB4
	NOT_ON_TIMER,	// PB5
	NOT_ON_TIMER,	// PB6
	NOT_ON_TIMER,	// PB7
	NOT_ON_TIMER,	// PD0
	NOT_ON_TIMER,	// PD1
	NOT_ON_TIMER,	// PD2
	NOT_ON_TIMER,	// PD3
	NOT_ON_TIMER,	// PD4
	TIMER0B,			// PD5
	NOT_ON_TIMER,	// PD6
};


#endif

#endif
