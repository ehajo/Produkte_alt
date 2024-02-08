/*
 * atevaltest.c
 *
 * Created: 23.09.2013 20:30:16
 *  Author: calm
 */ 
#define i2c_active 0 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "atevaltest.h"

#include "../usart/usart.h"
#include "../usart/usart_hlf.h"
#include "../gpio/gpio.h"
#include "../adc/adc.h"

#if i2c_active>0
#include "../i2ceeprom/i2ceeprom.h"
#endif

#include "../music/music.h"


volatile uint8_t zeilenposition=0;

volatile char recived_char='\0';

volatile emTestResult tater1_okay=Fehler;
volatile emTestResult tater2_okay=Fehler;
volatile emTestResult tater3_okay=Fehler;
volatile emTestResult led1_okay=Fehler;
volatile emTestResult led2_okay=Fehler;

volatile emTestResult poti1_okay=Fehler;
volatile emTestResult poti2_okay=Fehler;

#if i2c_active > 0
volatile emTestResult i2c_okay=Fehler;
#endif

volatile emTestResult speaker_okay=Fehler;

void pc_based_test(void);
void print_new_line(void);


void read_from_uart()
{
	recived_char=UDR;
}
void wait_for_enter()
{
	recived_char='\0';
	while(recived_char!=13)
	{
		
	}
}
uint8_t wait_for_key()
{
	recived_char=255;
	while(255==recived_char)
	{
		
	}
	return recived_char;
}
uint8_t ja_oder_nein()
{
	recived_char='\0';
	while((recived_char!='j')&&(recived_char!='n'))
	{
		
	}
	usart_putc(recived_char);
	return recived_char;
}
void print_full_line()
{
	for(uint8_t i=0;i<80;i++)
	{
		usart_putc('#');
	}
	print_new_line();
}
void print_empty_line_char(char x)
{
	usart_putc(x);
	for(uint8_t i=0;i<78;i++)
	{
		usart_putc(' ');
	}
	usart_putc(x);
	print_new_line();
}
void print_empty_line()
{
	print_empty_line_char('#');
}
void print_empty_line_error()
{
	print_empty_line_char('!');
}

const char string4[] EEMEM = "Weiter mit Enter";
void print_weiter_mit_enter_char(char x)
{
	usart_putc(x);
	for(uint8_t i=0;i<28;i++)
	{
		usart_putc(' ');
	}
	//usart_putp(PSTR("Weiter mit Enter"));
	usart_pute(string4);
	for(uint8_t i=0;i<34;i++)
	{
		usart_putc(' ');
	}
	usart_putc(x);
	print_new_line();
}
void print_weiter_mit_enter()
{
	print_weiter_mit_enter_char('#');
}
void print_weiter_mit_enter_error()
{
	print_weiter_mit_enter_char('!');
}
void print_full_line_error()
{
	for(uint8_t i=0;i<80;i++)
	{
		usart_putc('!');
	}
	print_new_line();
}
void print_new_line()
{
	usart_putp(PSTR("\n\r"));
}
void print_empty_lines()
{
	//3 neue Zeile
	print_new_line();
	print_new_line();
	print_new_line();
}

const char string1[] EEMEM = " Okay\n\r";

void print_okay()
{
	//usart_putp(PSTR(" Okay\n\r"));
	usart_pute(string1);
}

const char string2[] EEMEM = " Fehler bei Taste";
void print_fehler_bei_Taste()
{
	 //usart_putp(PSTR(" Fehler bei Taste"));
	 usart_pute(string2);
}

const char string3[] EEMEM = " ";

void print_text_to_line_end(char Borderchar)
{
	//usart_putp(PSTR(" "));
	usart_pute(string3);
	usart_putc(Borderchar);
	
}

void print_text_to_line_start(char Borderchar)
{
	usart_putc(Borderchar);
	usart_putp(PSTR(" "));
}


void fill_line(char Borderchar)
{
	//Einmal die Zeile zuendezeichen
	if(zeilenposition!=0)
	{
		while(zeilenposition<78)
		{
			usart_putc(' ');
			zeilenposition++;
		}
		print_text_to_line_end(Borderchar);
		print_new_line();
	}
	zeilenposition=0;
}



void print_text(const char *string,char Borderchar, char Stringtype)
{
	uint8_t zeichen='\0';
	if((Stringtype=='e')||(Stringtype=='E'))
	{
		zeichen=eeprom_read_byte((const uint8_t*)string);
	}
	else
	{
		zeichen=pgm_read_byte (string);	
	}
	
	while(zeichen!='\0')
	{
		
		if((zeilenposition==0)||(zeilenposition==77))
		{
			if(zeilenposition==77)
			{
				print_text_to_line_end(Borderchar);
			}
			else
			{
				print_text_to_line_start(Borderchar);
			}
			
			
			zeilenposition+=1;
			if(zeilenposition==78)
			{
				print_new_line();
				zeilenposition=0;
			}
		}
		else
		{
			if((Stringtype=='e')||(Stringtype=='E'))
			{
				zeichen=eeprom_read_byte((const uint8_t*)string);
			}
			else
			{
				zeichen=pgm_read_byte (string);
			}
			usart_putc(zeichen);
			string++;
			zeilenposition++;
		}
		
		
		
	}
}


void print_text_to_line(const char *string, char Borderchar, char Pointertype)
{
	if(zeilenposition!=0)
	{
		fill_line(Borderchar);
	}
	zeilenposition=0;
	print_text(string,Borderchar,Pointertype);
	
	fill_line(Borderchar);
	
	
}

void print_ehajo_danke()
{
	print_text_to_line(PSTR("Danke, dass Sie sich fuer das aTeVaL-Board von eHaJo.de entschieden haben."),'#','F');
}

void print_pruefen_sie_die_Platine()
{
	print_text_to_line(PSTR("Bitte pruefen Sie noch einmal die Platine"),'!','F');
}

const char string5[] EEMEM = "Potentiometer";
void print_potentiometer()
{
	//usart_putp(PSTR("Potentiometer"));
	usart_pute(string5);
}

void ateval_consoletest()
{
	Set_RX_Hook(read_from_uart);
	//Begrüßungstext
	print_full_line();
	print_ehajo_danke();
	print_text_to_line(PSTR("Mit diesem kleinen Programm kann nun die gerade aufgebaute Hardware getestetwerden. "),'#','F');
	print_empty_line();
	print_weiter_mit_enter();
	print_full_line();
	print_empty_lines();
	if(wait_for_key()!=13)
	{
		Delete_RX_Hook();
	}
	else
	{
		
		Test_Taster();
		Test_Leds();
		ateval_adctest();
		
		#if i2c_active > 0
		ateval_i2ctest();
		#endif
		
		ateval_speaktest();
		
		Zusammenfassung();
		
	}	
	
	
}

const char string6[] EEMEM = "Leuchtet LED1 jetzt? (j/n):";
const char string7[] EEMEM = "Leuchtet LED2 jetzt? (j/n):";
void Test_Leds()
{
	uint8_t errorcount=0;
	print_full_line();
    print_text_to_line(PSTR("Es werden nun die LED1 und LED2 getestet"),'#','F');
	print_full_line();
	print_empty_lines();
	Set_Led1(1);
	//usart_putp(PSTR("Leuchtet LED1 jetzt? (j/n):"));
	usart_pute(string6);
	if(ja_oder_nein()=='n')
	{
		errorcount++;
	}
	else
	{
		led1_okay=Okay;
	}
	usart_putp(PSTR("\n\r"));
	Set_Led1(0);
	
	Set_Led2(1);
	//usart_putp(PSTR("Leuchtet LED2 jetzt? (j/n):"));
	usart_pute(string7);
	
	if(ja_oder_nein()=='n')
	{
		errorcount++;
	}
	else
	{
		led2_okay=Okay;
	}
	print_new_line();
	Set_Led2(0);
	print_empty_lines();
	if(errorcount==0)
	{
		print_full_line();
		print_empty_line();
		print_text_to_line(PSTR("Sie haben die LED1 und LED2 erfolgreich getestet, gut gemacht."),'#','F');
		print_empty_line();
		print_full_line();
		print_empty_lines();
	}
	else
	{
		print_full_line_error();
		print_empty_line_error();
		print_text_to_line(PSTR("Es tut mir leid, aber die LED1 oder LED2 leuchtet nicht"),'!','F');
		print_pruefen_sie_die_Platine();
		print_empty_line_error();
		print_full_line_error();
		print_empty_lines();
		
	}
	
}

const char string8[] EEMEM = "Bitte nun S1 druecken:";
const char string9[] EEMEM = "Bitte nun S2 druecken:";
const char string10[] EEMEM = "Bitte nun S3 druecken:";
const char string20[] EEMEM = "1\n\r";
const char string21[] EEMEM = "2\n\r";
const char string22[] EEMEM = "3\n\r";
const char string23[] EEMEM = "Zuerst testen wir die Taster auf dem aTeVal";
const char string24[] EEMEM = "Die Taster sind auf der Platine mit S1, S2 und S3 beschriftet";

void Test_Taster()
{
	uint8_t counter =0;
	uint8_t errorcount=0;
	print_full_line();
	//print_text_to_line(PSTR("Zuerst testen wir die Taster auf dem aTeVal"),'#','F');
	print_text_to_line(string23,'#','E');
	//print_text_to_line(PSTR("Die Taster sind auf der Platine mit S1, S2 und S3 beschriftet."),'#','F');
	print_text_to_line(string24,'#','E');
	print_weiter_mit_enter();
	print_full_line();
	print_empty_lines();
	wait_for_enter();
	//usart_putp(PSTR("Bitte nun Taster1 druecken:"));
	usart_pute(string8);
	while((counter<250))
	{
		if(Get_Taster1()!=0)
		{
			break;
		}
		_delay_ms(25);
		counter++;
	}
	if((counter>=250)||(counter<1))
	{
		print_fehler_bei_Taste();
		//usart_putp(PSTR("1\n\r"));
		usart_pute(string20);
		errorcount++;
	}
	else
	{
		print_okay();
		tater1_okay=Okay;
	}
	counter=0;
	
	//usart_putp(PSTR("Bitte nun Taster2 druecken:"));
	usart_pute(string9);
	while(counter<250)
	{
		if(Get_Taster2()!=0)
		{
			break;
		}
		_delay_ms(25);
		counter++;
	}
	if((counter>=250)||(counter<1))
	{
		print_fehler_bei_Taste();
		//usart_putp(PSTR("2\n\r"));
		usart_pute(string21);
		errorcount++;
	}
	else
	{
		print_okay();
		tater2_okay=Okay;
	}
	counter=0;
	
	//usart_putp(PSTR("Bitte nun Taster3 druecken:"));
	usart_pute(string10);
	while(counter<250)
	{
		if(Get_Taster3()!=0)
		{
			break;
		}
		_delay_ms(25);
		counter++;
	}
	if((counter>=250)||(counter<1))
	{
		print_fehler_bei_Taste();
		//usart_putp(PSTR("3\n\r"));
		usart_pute(string22);
		errorcount++;
	}
	else
	{
		print_okay();
		tater3_okay=Okay;
	}
	counter=0;
	
	print_empty_lines();

	if(errorcount==0)
	{
		print_full_line();
		print_empty_line();
		print_text_to_line(PSTR("Sie haben die Taster1 bis Taster3 erfolgreich getestet, gut gemacht."),'#','F');
		print_weiter_mit_enter();
		print_empty_line();
		print_full_line();
		
	}
	else
	{
		print_full_line_error();
		print_empty_line_error();
		//usart_putp(PSTR("!  Es tut mir leid, aber die Taster arbeiten nicht korrekt, bitte pruefen Sie  !\n\r"));
		//usart_putp(PSTR("!  noch einmal die Platine                                                     !\n\r"));
		print_text_to_line(PSTR("Es tut mir leid, aber die Taster arbeiten nicht korrekt."),'!','F');
		print_pruefen_sie_die_Platine();
		print_weiter_mit_enter_error();
		print_full_line_error();
	}
	print_empty_lines();
	wait_for_enter();
}

const char string11[] EEMEM = "Wollen Sie den Test jetzt starten? (j/n):";
//const char string30[] EEMEM = "Falls Sie ein IIC EEPROM im Sockel IC6 stecken haben kann nun die Funktion  getestet werden. Die Daten auf dem EEPROM bleiben erhalten es werden keine  Daten auf das EEPROM geschrieben";
const char string31[] EEMEM="Sie haben die das IIC-EEPROM erfolgreich getestet, gut gemacht.";
#if i2c_active > 0
void ateval_i2ctest()
{
	print_full_line();
	print_empty_line();
	print_text_to_line(PSTR("Falls Sie ein IIC EEPROM im Sockel IC6 stecken haben kann nun die Funktion  getestet werden. Die Daten auf dem EEPROM bleiben erhalten es werden keine  Daten auf das EEPROM geschrieben"),'#','F');
	//print_text_to_line(string30,'#','E');
	print_empty_line();
	print_full_line();
	print_empty_lines();
	//usart_putp(PSTR("Wollen Sie den Test jetzt starten? (j/n):"));
	usart_pute(string11);
	
	if(ja_oder_nein()=='j')
	{
		print_empty_lines();
		if(i2c_bus_test(0xA0)!=0)
		{
			print_full_line_error();
			print_empty_line_error();
			print_text_to_line(PSTR("Es tut mir leid, aber das IIC-EEPROM kann nicht angesprochen wernde,"),'!','F');
			print_pruefen_sie_die_Platine();
		
		
			print_empty_line_error();
			print_weiter_mit_enter();
			print_full_line_error();
			wait_for_enter();
		}
		else
		{
			print_full_line();
			print_empty_line();
			//usart_putp(PSTR("#  Sie haben die das IIC-EEPROM erfolgreich getestet, gut gemacht.             #\n\r"));
			
			//print_text_to_line(PSTR("Sie haben die das IIC-EEPROM erfolgreich getestet, gut gemacht."),'#','F');
			print_text_to_line(string31,'#','E');
			print_empty_line();
			print_weiter_mit_enter();
			print_full_line();
			wait_for_enter();
			i2c_okay=Okay;
		}
		print_empty_lines();
	}
	else
	{
		print_empty_lines();
		print_full_line();
		print_empty_line();
		//usart_putp(PSTR("#                            Test uebergangen		                            #\n\r"));
		print_text_to_line(PSTR("Test uebergangen"),'#','F');
		print_full_line();
		i2c_okay=Ueberspungen;
	}
	
	
}
#endif
const char string12[] EEMEM="Haben Sie einen Ton gehoert? (j/n):";
void ateval_speaktest()
{
	print_full_line();
	print_empty_line();
	//usart_putp(PSTR("#                 Es wird nun der Lautsprecher getestet	                        #\n\r"));
	print_text_to_line(PSTR("Es wird nun der Lautsprecher getestet"),'#','F');
		
	
	print_weiter_mit_enter();
	print_empty_line();
	print_full_line();
	wait_for_enter();
	
	//music_play();
	play_tone();
	print_empty_lines();
	//usart_putp(PSTR("Haben Sie einen Ton gehoert? (j/n):"));
	usart_pute(string12);
	if(ja_oder_nein()=='j')
	{
		print_empty_lines();
		print_full_line();
		print_empty_line();
		//usart_putp(PSTR("#  Sie haben den Lautsprecher erfolgreich getestet, ich hoffe die Melodie      #\n\r"));
		//usart_putp(PSTR("#  hat Ihnen gefallen.                                                         #\n\r"));
		
		print_text_to_line(PSTR("Sie haben den Lautsprecher erfolgreich getestet"),'#','F');
		
		print_weiter_mit_enter();
		print_full_line();
		wait_for_enter();
		speaker_okay=Okay;;
	}
	else
	{
		print_empty_lines();
		print_full_line_error();
		print_empty_line_error();
	
		print_text_to_line(PSTR("Es tut mir leid, aber der Lautsprecher scheint keinen Ton von sich zu geben,"),'!','F');
		print_pruefen_sie_die_Platine();
		
		print_empty_line_error();
		print_weiter_mit_enter_error();
		print_full_line_error();
		wait_for_enter();
	}
	print_empty_lines();
	
}

void ateval_adctest()
{
	print_full_line();
	print_empty_line();
	//usart_putp(PSTR("#  Es wird nun das Potentiometer1 mit der Beschriftung R1 und das              #\n\r"));
	//usart_putp(PSTR("#  Potentiometer2 mit der Beschriftung R2 getestet.                             #\n\r"));
	
	print_text_to_line(PSTR("Es wird nun das Potentiometer1 mit der Beschriftung R1 und das"),'#','F');
	print_text_to_line(PSTR("Potentiometer2 mit der Beschriftung R2 getestet."),'#','F');
	
	print_weiter_mit_enter();
	print_full_line();
	wait_for_enter();
	print_empty_lines();
	adc_potentiometercheck(0);
	adc_potentiometercheck(1);
	if(poti1_okay == Fehler || poti2_okay==Fehler)
	{
		print_empty_lines();
		print_full_line_error();
		print_empty_line_error();
		
		print_text_to_line(PSTR("Es tut mir leid, aber der Potentioneter1 oder 2 haben einen Fehler,"),'!','F');
		print_pruefen_sie_die_Platine();
		
		print_empty_line_error();
		print_weiter_mit_enter_error();
		print_full_line_error();
		wait_for_enter();
	}
	
}
#define ADCTIMEOUT 3000
const char string13[] EEMEM = "Bitte drehen Sie das ";
const char string14[] EEMEM = " ganz nach ";
const char string15[] EEMEM = "links";
const char string16[] EEMEM = "rechts";
void adc_potentiometercheck(uint8_t Potentiometernummer)
{
	uint8_t adc_channel=0;
	
	if(Potentiometernummer==0)
	{
		adc_channel=1;
	}
	uint16_t adc_value = Get_ADC_Value(adc_channel);
	uint16_t counter=0;
	
	if(Get_ADC_Value(adc_channel)<21)
	{
		//usart_putp(PSTR("Bitte drehen Sie das"));
		usart_pute(string13);
		print_potentiometer();
		if(Potentiometernummer==0)
		{
			usart_putp(PSTR("1"));
		}
		else
		{
			usart_putp(PSTR("2"));
		}
		//usart_putp(PSTR(" ganz nach links\n\r"));
		usart_pute(string14);
		usart_pute(string15);
		print_new_line();
		adc_value = Get_ADC_Value(adc_channel);
		counter=0;
		while((adc_value<0x2AF)&&(counter<ADCTIMEOUT))
		{
			_delay_ms(10);
			adc_value=Get_ADC_Value(adc_channel);
			counter++;
		
		}
		if(counter<ADCTIMEOUT)
		{
			counter=0;
			//usart_putp(PSTR("Bitte drehen Sie das "));
			usart_pute(string13);
			print_potentiometer();
			if(Potentiometernummer==0)
			{
				usart_putp(PSTR("1"));
			}
			else
			{
				usart_putp(PSTR("2"));
			}
			//usart_putp(PSTR(" ganz nach rechts\n\r"));
			usart_pute(string14);
			usart_pute(string16);
			print_new_line();
			
			while((adc_value>20)&&(counter<ADCTIMEOUT))
			{
				_delay_ms(10);
				adc_value=Get_ADC_Value(adc_channel);
				counter++;
			}
			//OKay weiter im Text
			if(counter<ADCTIMEOUT)
			{
				print_potentiometer();
				if(Potentiometernummer==0)
				{
					usart_putp(PSTR("1"));
				}
				else
				{
					usart_putp(PSTR("2"));
				}
				//usart_putp(PSTR(": okay\n\r"));
				print_okay();
				if(Potentiometernummer==0)
				{
					poti1_okay=Okay;
				}
				else
				{
					poti2_okay=Okay;
				}
		    }
			
		}
	
	
	}
	//if(Get_ADC_Value(adc_channel)<21)
	else
	{
		usart_pute(string13);
		print_potentiometer();
		if(Potentiometernummer==0)
		{
			usart_putp(PSTR("1"));
		}
		else
		{
			usart_putp(PSTR("2"));
		}
		//usart_putp(PSTR(" ganz nach rechts\n\r"));
		usart_pute(string14);
		usart_pute(string16);
		print_new_line();
		
		counter=0;
		while((adc_value>20)&&(counter<ADCTIMEOUT))
		{
			_delay_ms(10);
			adc_value=Get_ADC_Value(adc_channel);
			counter++;
		
		}
		if(counter<ADCTIMEOUT)
		{
			counter=0;
			usart_pute(string13);
			print_potentiometer();
			if(Potentiometernummer==0)
			{
				usart_putp(PSTR("1"));
			}
			else
			{
				usart_putp(PSTR("2"));
			}
			//usart_putp(PSTR(" ganz nach links\n\r"));
			usart_pute(string14);
			usart_pute(string15);
			print_new_line();
			
			while((adc_value<0x2AF)&&(counter<ADCTIMEOUT))
			{
				_delay_ms(10);
				adc_value=Get_ADC_Value(adc_channel);
				counter++;
			}
			//OKay weiter im Text
			if(counter<ADCTIMEOUT)
			{
				print_potentiometer();
				if(Potentiometernummer==0)
				{
					usart_putp(PSTR("1"));
				}
				else
				{
					usart_putp(PSTR("2"));
				}
				print_okay();
				if(Potentiometernummer==0)
				{
					poti1_okay=Okay;
				}
				else
				{
					poti2_okay=Okay;
				}
			}
		}
		
	}
	
	
	
	
	
	
	
	
	
	
}

void print_eins()
{
	print_text(PSTR("1"),'#','F');
}

void print_zwei()
{
	print_text(PSTR("2"),'#','F');
}

void print_drei()
{
	print_text(PSTR("3"),'#','F');
	
}

void print_okay_zf()
{
	print_text(PSTR(": Okay"),'#','F');
}

void print_taster()
{
	print_text(PSTR("Taster"),'#','F');
}

void print_fehler()
{
	print_text(PSTR(": Fehler"),'#','F');
}

void print_led()
{
	print_text(PSTR("Led"),'#','F');
}

void print_i2c_eeprom()
{
	print_text(PSTR("I2C-EEPROM:"),'#','F');
	
}
void Zusammenfassung()
{
	for(uint8_t i=0;i<10;i++);
	{
		print_empty_lines();
	}
		print_full_line();
		print_empty_line();
		print_ehajo_danke();
		print_text_to_line(PSTR("Zusammenfassung des Testlaufs"),'#','F');
			
		//Taster 1
		print_taster();
		print_eins();
		zeilenposition-=2;
		if(tater1_okay==Okay)
		{
			print_okay_zf();
		
		}
		else
		{
			print_fehler();
		}
		
		fill_line('#');
				
		print_taster();
		print_zwei();
		zeilenposition-=2;
		if(tater2_okay==Okay)
		{
			print_okay_zf();
			
		}
		else
		{
			print_fehler();
		}
		
		fill_line('#');
		print_taster();
		print_drei();
		if(tater3_okay==Okay)
		{
			print_okay_zf();
			
		}
		else
		{
			print_fehler();
		}
		                                                            
		zeilenposition-=2;
		fill_line('#');
		print_led();
		print_eins();
		if(led1_okay==Okay)
		{
			print_okay_zf();
		}
		else
		{
			print_fehler();
		}
		zeilenposition-=2;
		fill_line('#');
		
		
		print_led();
		print_zwei();
		if(led1_okay==Okay)
		{
			print_okay_zf();
		}
		else
		{
			print_fehler();
		}
		zeilenposition-=2;
		fill_line('#');
		
	
	
	print_text_to_line_start('#');
	print_potentiometer();
	usart_puts(" 1");
	zeilenposition=16;
	if(poti1_okay==Okay)
	{
		print_okay_zf();
	}
	else
	{
		print_fehler();
	}
	fill_line('#');
	
	
	print_text_to_line_start('#');
	print_potentiometer();
	usart_puts(" 2");
	zeilenposition=16;
	if(poti2_okay==Okay)
	{
		print_okay_zf();
	}
	else
	{
		print_fehler();
	}
		
	fill_line('#');
	
	#if i2c_active > 0
	print_i2c_eeprom();
	zeilenposition-=1;
	if(i2c_okay==Okay)
	{
		//usart_putp(PSTR("#  I2C-EEPROM: Okay                                                            #\n\r"));
		print_okay_zf();
	}
	else if(i2c_okay==Ueberspungen)
	{
		//usart_putp(PSTR("#  I2C-EEPROM: Uebersprungen                                                   #\n\r"));
		print_text(PSTR(" Uebersprungen"),'#','F');
	}
	else
	{ 
		print_fehler();
	}
	fill_line('#');
	#endif
	
	if(speaker_okay==Okay)
	{
		//usart_putp(PSTR("#  Lautsprecher: Okay                                                          #\n\r"));	
		print_text_to_line(PSTR("Lautsprecher: Okay"),'#','F');
	}
	else
	{
		//usart_putp(PSTR("#  Lautsprecher: Fehler                                                        #\n\r"));	
		print_text_to_line(PSTR("Lautsprecher: Fehler"),'#','F');
	}
	
	print_empty_line();
	print_full_line();
	print_empty_lines();
	print_empty_lines();
	
}