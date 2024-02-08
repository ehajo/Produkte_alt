/*
 * gpio.h
 *
 * Created: 20.09.2013 14:53:45
 *  Author: calm
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>


#define LED1 PORTD6
#define LED2 PORTD5

#define Taster1 PORTD2
#define Taster2 PORTD3
#define Taster3 PORTD4

#define Summer PORTD7



void GPIO_Init(void);

void Set_Led1(uint8_t Zustand);

uint8_t Get_Led1(void);

void Set_Led2(uint8_t Zustand);

uint8_t Get_Led2(void);

void Set_Summer(uint8_t Zustand);

void Toggle_Summer(void);

uint8_t Get_Taster1(void);

uint8_t Get_Taster2(void);

uint8_t Get_Taster3(void);



#endif /* GPIO_H_ */