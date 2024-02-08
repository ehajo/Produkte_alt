/*
 * i2ceeprom.h
 *
 * Created: 21.09.2013 18:15:02
 *  Author: calm
 */ 


#ifndef I2CEEPROM_H_
#define I2CEEPROM_H_

#include "../i2c/i2cmaster.h"

uint8_t i2c_bus_test(uint8_t DeviceAddess);

uint8_t write_string_to_eeprom(uint8_t DeviceAddess, uint8_t startaddress, const char* String);
uint8_t read_string_from_eeprom(uint8_t DeviceAddess, uint8_t startaddress, char* Buffer, uint16_t Buffersize);
uint8_t ReadByteFromEEPROM(uint8_t DeviceAddess, uint8_t StartByte);
void WriteByteToEEPROM(uint8_t DeviceAddess, uint8_t StartByte, uint8_t DataByte);

uint8_t ReadWordFromEEPROM(uint8_t DeviceAddess, uint8_t StartByte, uint8_t* DataBytes);
void WriteWordToEEPROM(uint8_t DeviceAddess, uint8_t StartWord, uint8_t* DataBytes);

#endif /* I2CEEPROM_H_ */