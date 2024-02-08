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

  Modified by eHaJo for our development-boards
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            15

#define digitalPinHasPWM(p)         ((p) == 5 || (p) == 9 || (p) == 10 || (p) == 11)

#define PIN_SPI_SS    (11)
#define PIN_SPI_MOSI  (12)
#define PIN_SPI_MISO  (13)
#define PIN_SPI_SCK   (14)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

#define PIN_WIRE_SDA        (11)
#define PIN_WIRE_SCL        (13)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 13

#define digitalPinToPCICR(p)    (((p) >= 7 && (p) <= 14) ? (&GIMSK) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) >= 7 && (p) <= 14) ? 0x20 : 0))
#define digitalPinToPCMSK(p)    (((p) >= 7 && (p) <= 14) ? (&PCMSK) : ((uint8_t *)0))
#define digitalPinToPCMSKbit(p) (((p) >= 7 && (p) <= 14) ? (p)-7 : 0)

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : ((p) == 3 ? 1 : NOT_AN_INTERRUPT))


#ifdef ARDUINO_MAIN

// ATMEL ATTiny 2313
//
//                  +-\/-+
//      (RST) PA2  1|    |20  VCC
//      (D 0) PD0  2|    |19  PB7 (D 13)
//      (D 1) PD1  3|    |18  PB6 (D 12)
//            PA1  4|    |17  PB5 (D 11)
//            PA0  5|    |16  PB4 (D 10)
//      (D 2) PD2  6|    |15  PB3 (D  9)
//      (D 3) PD3  7|    |14  PB2 (D 14)
//      (D 4) PD4  8|    |13  PB1 (D  7)
//      (D 5) PD5  9|    |12  PB0 (D  8)
//            GND 10|    |11  PD6 (D  6)
//                  +----+



const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,			// PortA not used: reset and XTAL
	(uint16_t) &DDRB,
	NOT_A_PORT,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	NOT_A_PORT,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	NOT_A_PORT,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD,	// 0	PD0
	PD,	// 1	PD1
	PD,	// 2	PD2
	PD,	// 3	PD3
	PD,	// 4	PD4
	PD,	// 5	PD5
	PD,	// 6	PD6
	PB,	// 7	PB1
	PB,	// 8	PB0
	PB,	// 9	PB3
	PB,	// 10	PB4
	PB,	// 11	PB5
	PB,	// 12	PB6
	PB,	// 13	PB7
	PB,	// 14	PB2
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0),	// PD0
	_BV(1),	// PD1
	_BV(2),	// PD2
	_BV(3),	// PD3
	_BV(4),	// PD4
	_BV(5),	// PD5
	_BV(6),	// PD6
	_BV(1),	// PB1
	_BV(0),	// PB0
	_BV(3),	// PB3
	_BV(4),	// PB4
	_BV(5),	// PB5
	_BV(6),	// PB6
	_BV(7),	// PB7
	_BV(2),	// PB2
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER,	// PD0
	NOT_ON_TIMER,	// PD1
	NOT_ON_TIMER,	// PD2
	NOT_ON_TIMER,	// PD3
	NOT_ON_TIMER,	// PD4
	TIMER0B,		// PD5
	NOT_ON_TIMER,	// PD6
	NOT_ON_TIMER,	// PB1
	NOT_ON_TIMER,	// PB0
	TIMER1A,		// PB3
	TIMER1B,		// PB4
	NOT_ON_TIMER,	// PB5
	NOT_ON_TIMER,	// PB6
	NOT_ON_TIMER,	// PB7
	TIMER0A,		// PB2
};


#endif

// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//
#define SERIAL_PORT_MONITOR   Serial
#define SERIAL_PORT_HARDWARE  Serial

#endif
