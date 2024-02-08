/*
 * atevaltest.h
 *
 * Created: 23.09.2013 20:30:29
 *  Author: calm
 */ 


#ifndef ATEVALTEST_H_
#define ATEVALTEST_H_

void ateval_consoletest(void);
void Test_Taster(void);
void Test_Leds(void);
void ateval_i2ctest(void);
void ateval_speaktest(void);
void ateval_adctest(void);
void adc_potentiometercheck(uint8_t);
void Zusammenfassung(void);
typedef enum {Okay,Fehler,Ueberspungen} emTestResult;

#endif /* ATEVALTEST_H_ */