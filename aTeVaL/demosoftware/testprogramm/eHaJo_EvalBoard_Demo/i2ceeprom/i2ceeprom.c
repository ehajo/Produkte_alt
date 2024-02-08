/*
 * i2ceeprom.c
 *
 * Created: 21.09.2013 18:14:51
 *  Author: calm
 */ 

#include "../i2ceeprom/i2ceeprom.h"
#include <avr/pgmspace.h>


uint8_t i2c_bus_test(uint8_t DeviceAddess)
{
	uint8_t response=0;
	response += i2c_start(DeviceAddess+I2C_WRITE);
	response += i2c_write(0);						 // write address = 0
	response += i2c_rep_start(DeviceAddess+I2C_READ);       // set device address and read mode
	i2c_readAck();
	return response;
	
}


//Okay we address only one 256byte EEPROM so we got 128 Words at max
uint8_t ReadByteFromEEPROM(uint8_t DeviceAddess, uint8_t StartByte) 
{
	if(i2c_start(DeviceAddess+I2C_WRITE)==0)     // set device address and write mode
	{
		i2c_write(StartByte);
		i2c_rep_start(DeviceAddess+I2C_READ);       // set device address and read mode
		return i2c_readNak();	
	}
	else
	{
		return 0xFF;
	}
	
}

uint8_t ReadWordFromEEPROM(uint8_t DeviceAddess, uint8_t StartWord, uint8_t* DataBytes)
{
	StartWord=StartWord&0x7F;
	StartWord=StartWord<<1;
	if(i2c_start(DeviceAddess+I2C_WRITE)==0)     // set device address and write mode
	{
		i2c_write(StartWord);
		i2c_rep_start(DeviceAddess+I2C_READ);       // set device address and read mode
		*DataBytes= i2c_readAck();
		DataBytes++;
		*DataBytes= i2c_readNak();
		return 0;
	}
	else
	{
		return 1;
	}
	
}

void WriteByteToEEPROM(uint8_t DeviceAddess, uint8_t StartByte, uint8_t DataByte) 
{

		if(i2c_start(DeviceAddess+I2C_WRITE)==0)     // set device address and write mode
		{
			i2c_write(StartByte);                    // write address = 0
			i2c_write(DataByte);
			i2c_stop();	
		}
		
}

void WriteWordToEEPROM(uint8_t DeviceAddess, uint8_t StartWord, uint8_t* DataBytes)
{

	StartWord=StartWord&0x7F;
	StartWord=StartWord<<1;
	if(i2c_start(DeviceAddess+I2C_WRITE)==0)     // set device address and write mode
	{
		i2c_write(StartWord);                    // write address = 0
		i2c_write(*DataBytes++);
		i2c_write(*DataBytes);
		i2c_stop();
	}
	
}



uint8_t write_string_to_eeprom(uint8_t DeviceAddess, uint8_t startaddress, const char* String)
{
	while (*String) {
		//Singel Bytewrite to overcome pagelimits of eeprom
		i2c_start_wait(DeviceAddess+I2C_WRITE);     // set device address and write mode
		i2c_write(startaddress++);                    // write address = 0
		i2c_write(*String);
		i2c_stop();
		String++;
	}
	i2c_start_wait(DeviceAddess+I2C_WRITE);     // set device address and write mode
	i2c_write(startaddress++);                    // write address = 0
	i2c_write('\0');
	i2c_stop();
	return 0;                            // set stop conditon = release bus
}

uint8_t read_string_from_eeprom(uint8_t DeviceAddess, uint8_t startaddress, char* Buffer, uint16_t Buffersize)
{
	uint8_t tempchar ='\0';
	uint16_t Bufferposition=0;
	uint8_t readdata=1;
	i2c_start_wait(DeviceAddess+I2C_WRITE);     // set device address and write mode
	i2c_write(startaddress);                    
	i2c_rep_start(DeviceAddess+I2C_READ);       // set device address and read mode
	tempchar=i2c_readAck();
	
	while(readdata!=0)
	{
		if(tempchar!='\0')
		{
			if(Bufferposition<(Buffersize-1))
			{
				Buffer[Bufferposition++]=tempchar;
			}
			tempchar=i2c_readAck();
		}
		else
		{
			if(Bufferposition<(Buffersize-1))
			{
				Buffer[Bufferposition++]=tempchar;
			}
			i2c_readNak();                    // read one byte from EEPROM
			i2c_stop();	
			readdata=0;	
		}
	}
	
	return 0;
	
}