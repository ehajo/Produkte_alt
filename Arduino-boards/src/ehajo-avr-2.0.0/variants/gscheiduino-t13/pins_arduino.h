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
                  +-\/-+
      (RST) PB5  1|    |8  VCC
      (D 3) PB3  2|    |7  PB2 (D 2)
      (D 4) PB4  3|    |6  PB1 (D 1)
            GND  4|    |5  PB0 (D 0)
                  +----+
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            5
#define NUM_ANALOG_INPUTS           2
#define analogInputToDigitalPin(p)  ((p < 2) ? p : -1)

#define TIMER0_OVF_vect TIM0_OVF_vect

#define digitalPinHasPWM(p)         ((p) == 0 || (p) == 1)

#define PIN_SPI_SS    (3)
#define PIN_SPI_MOSI  (0)
#define PIN_SPI_MISO  (1)
#define PIN_SPI_SCK   (2)

static const uint8_t SS   = PIN_SPI_SS;
static const uint8_t MOSI = PIN_SPI_MOSI;
static const uint8_t MISO = PIN_SPI_MISO;
static const uint8_t SCK  = PIN_SPI_SCK;

#define PIN_A0   (0)
#define PIN_A1   (1)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;

#define LED_BUILTIN 13 

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 5) ? (&GIMSK) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 5) ? 0x20 : 0))
#define digitalPinToPCMSK(p)    (((p) <= 5) ? (&PCMSK) : ((uint8_t *)0))
#define digitalPinToPCMSKbit(p) (p)

#define digitalPinToInterrupt(p)  ((p) == 2 ? 0 : (NOT_AN_INTERRUPT))

#ifdef ARDUINO_MAIN

const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	0,  /* 0 */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	PB, /* 11 */
	PB, /* 12 */
	PB, /* 13 */
	0,  /* 14 */
	0,  /* 15 */
	PB, /* 16 */
	PB, /* 17 */
	// PB5 nicht mappen, da reset
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	0,  /* 0 */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	_BV(0), /* 11, port B */
	_BV(1),
	_BV(2),
	0,
	0,
	_BV(4),
	_BV(3),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  TIMER0A, /* OC0A */
  TIMER0B,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
};

#endif

#endif
