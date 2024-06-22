#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C_Init(void);
uint8_t I2C_Start(void);
uint8_t I2C_Stop(void);
// uint8_t I2C_Write(uint8_t data);
uint8_t I2C_ReadAck(void);
uint8_t I2C_ReadNack(void);
// void I2C_WriteData(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data);
uint8_t I2C_ReadData(uint8_t deviceAddress, uint8_t registerAddress);

#endif // I2C_H
