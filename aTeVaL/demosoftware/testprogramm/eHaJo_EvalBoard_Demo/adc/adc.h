/*
 * adc.h
 *
 * Created: 20.09.2013 19:29:04
 *  Author: calm
 */ 


#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>

void adc_init(void);
uint16_t Get_ADC_Value(uint8_t);

#endif /* ADC_H_ */