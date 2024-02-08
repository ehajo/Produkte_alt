/*
 * eHaJo_EvalBoard_Demo.c
 *
 * Created: 14.09.2013 09:16:47
 *  Author: calm
 */ 

#define modbus_active 0
#define i2c_active 0

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "usart/usart.h"

#if i2c_active > 0 
//#include "usart_tools.h"
#endif

#include "gpio/gpio.h"
#include "adc/adc.h"


//#include "i2ceeprom/i2ceeprom.h"
#include "music/music.h"

#include "ateval_test/atevaltest.h"

#if modbus_active > 0
#include "modbus/include/mb.h"
#include "modbus/port/port.h"


#define REG_INPUT_START 1000
#define REG_INPUT_RAMREG 128
#define REG_INPUT_NREGS (520 + REG_INPUT_RAMREG)


volatile USHORT   usRegInputStart = REG_INPUT_START;
volatile USHORT   usRegInputBuf[REG_INPUT_RAMREG];
#endif
volatile uint8_t ledstatus=0;
volatile uint8_t ledstatus2=0;
int main(void)
{
	#if modbus_active >0
	const UCHAR     ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
	//Aus der Modbuslibary
	eMBErrorCode    eStatus;
	#endif
	
	usartparam_t Parameter;
	//9600 Baud bei 1MHz mit 2X Bit gesetzt 
	Parameter.BaudrateRegister=12; 
	Parameter.Doublespeed=1;
	//Und nun 8N1 als Datemformat einstellen
	Parameter.NoOfBits=Eight;
	Parameter.Paritytype= None;
	Parameter.NoOfStopbits = One;
	
	usart_init(&Parameter); //Die Parameter werden als Pointer übergeben
		
	
	//Es muss nun für das Baord die GPIO Pins passend Konfiguriert werden
	GPIO_Init();
	
	//Einmal den ADC initalisieren
	adc_init();
	
	#if i2c_active > 0
	i2c_init();                             // initialize I2C library
	#endif
	
	//Interrupts anschalten
	sei();
	ateval_consoletest();	
	cli();
	//Interrupts abschalten
	
	//Okay kein Testtext also einmal in den ModBus Mode umschalten
	#if modbus_active>0
	
	eStatus = eMBInit( MB_RTU, 0x0A, 0, 4800, MB_PAR_NONE );
    	
	eStatus = eMBSetSlaveID( 0x34, TRUE, ucSlaveID, 3 );
	sei();
	eMBEnable();
	while(1==1)
	{
		  (void) eMBPoll(  );
		  /* Here we simply count the number of poll cycles. */
			  
	}
	
	#else
	
	
	
	while(1==1)
	{
		
	}
	#endif
}

ISR(BADISR_vect)
{
	//Okay da geht was schief!
}

#if modbus_active > 0
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{

	
	return MB_ENOREG;
}

//EEPROM Routines

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
eMBRegisterMode eMode )
{
	
	
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;

	if( ( usAddress >= REG_INPUT_START )
	&& ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
	{
		//Okay we need to determin if we read or write to the register....
		iRegIndex = ( int )( usAddress - usRegInputStart );
		while( usNRegs > 0 ) 
		{
			if(eMode==MB_REG_READ)
			{
				#if i2c_active>0
				if(iRegIndex<128)
				{
					ReadWordFromEEPROM(0xA0,iRegIndex,pucRegBuffer);	
				}
				else if(iRegIndex<256)
				{
					ReadWordFromEEPROM(0xA2,iRegIndex,pucRegBuffer);	
				}
				else if(iRegIndex<384)
				{
					ReadWordFromEEPROM(0xA4,iRegIndex,pucRegBuffer);	
				}
				else if(iRegIndex<512)
				{
					ReadWordFromEEPROM(0xA8,iRegIndex,pucRegBuffer);	
				}
				else 
				#endif
				if(iRegIndex==512)
				{
					*pucRegBuffer=0;
					*(pucRegBuffer+1)=Get_Led1();
					
				}
				else if(iRegIndex==513)
				{
					*pucRegBuffer=0;
					*(pucRegBuffer+1)=Get_Led2();
					
				}
				else if(iRegIndex==514)
				{
					//Das hier wäre der Nuzzer
				}
				else if(iRegIndex==514)
				{
					uint16_t ADC_VAlue=Get_ADC_Value(0);
					*pucRegBuffer=(ADC_VAlue>>8);			  //Highbyte
					*(pucRegBuffer+1)=(uint8_t)(ADC_VAlue);   //Lowbyte
				}
				else if(iRegIndex==514)
				{
					uint16_t ADC_VAlue=Get_ADC_Value(1);
					*pucRegBuffer=(ADC_VAlue>>8);			  //Highbyte
					*(pucRegBuffer+1)=(uint8_t)(ADC_VAlue);   //Lowbyte
				}
				else if(iRegIndex==517)
				{
					*pucRegBuffer=0;
					*(pucRegBuffer+1)=Get_Taster1();
				
				}
				else if(iRegIndex==518)
				{
					*pucRegBuffer=0;
					*(pucRegBuffer+1)=Get_Taster2();
				}
				else if(iRegIndex==519)
				{
					*pucRegBuffer=0;
					*(pucRegBuffer+1)=Get_Taster3();
				}
				else if((iRegIndex>519)&&(iRegIndex<(519+REG_INPUT_RAMREG)))
				{
					*pucRegBuffer=usRegInputBuf[iRegIndex-520]>>8;
					*(pucRegBuffer+1)=(uint8_t)(usRegInputBuf[iRegIndex-520]);
				}
				pucRegBuffer++;
				pucRegBuffer++;
				iRegIndex++;
				usNRegs--;	
			}
			else
			{
				#if i2c_active>0
				if(iRegIndex<128)
				{
					WriteWordToEEPROM(0xA0,iRegIndex,pucRegBuffer);
				}
				else if(iRegIndex<256)
				{
					WriteWordToEEPROM(0xA2,iRegIndex,pucRegBuffer);
				}
				else if(iRegIndex<384)
				{
					WriteWordToEEPROM(0xA4,iRegIndex,pucRegBuffer);
				}
				else if(iRegIndex<512)
				{
					WriteWordToEEPROM(0xA8,iRegIndex,pucRegBuffer);
				}
				else 
				#endif
				if(iRegIndex==512)
				{
					if(*(pucRegBuffer+1)>0)
					{
						Set_Led1(1);
						
					}
					else
					{
						Set_Led1(0);
					}
				}
				else if(iRegIndex==513)
				{
					if(*(pucRegBuffer+1)>0)
					{
						Set_Led2(1);
						
					}
					else
					{
						Set_Led2(0);
					}
					
				}
				else if(iRegIndex==514)
				{
					//Das hier wäre der Buzzer ....
				}
				else if(iRegIndex==515)
				{
					//Das hier wäre ADC0, denn kann man nicht schreiben :-(
				}
				else if(iRegIndex==516)
				{
					//Das hier wäre ADC1 , denn kann man nicht schreiben :-(
					
				}
				else if(iRegIndex==517)
				{
					//Das hier wäre ADC1 , denn kann man nicht schreiben :-(
					
				}
				else if(iRegIndex==518)
				{
					//Das hier wäre ADC1 , denn kann man nicht schreiben :-(
					
				}
				else if(iRegIndex==519)
				{
					//Das hier wäre ADC1 , denn kann man nicht schreiben :-(
					
				}
				else if((iRegIndex>519)&&(iRegIndex<(519+REG_INPUT_RAMREG)))
				{
					usRegInputBuf[iRegIndex-520]=((*pucRegBuffer)<<8);
					usRegInputBuf[iRegIndex-520]|=*(pucRegBuffer+1);
				}
				pucRegBuffer++;
				pucRegBuffer++;
				
				iRegIndex++;
				usNRegs--;
				
			}
			
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}

	return eStatus;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
eMBRegisterMode eMode )
{
	return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
	return MB_ENOREG;
}
#endif