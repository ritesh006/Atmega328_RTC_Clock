#include "i2c.h"
#include "config.h"
#include <avr/io.h>
#include <util/twi.h>


void I2C_Init(void) {
  TWSR = 0x00;                           // Set prescaler to 1
  TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2; // Set bit rate register
  TWCR = (1 << TWEN);                    // Enable TWI
}

uint8_t I2C_Start(void) {
  TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT); // Send START condition
  while (!(TWCR & (1 << TWINT)))
    ;                   // Wait for TWINT flag to be set
  return (TWSR & 0xF8); // Return status
}

uint8_t I2C_Stop(void) {
  TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT); // Send STOP condition
  while (TWCR & (1 << TWSTO))
    ;                   // Wait for STOP to be executed
  return (TWSR & 0xF8); // Return status
}

uint8_t I2C_Write(uint8_t data) {
  TWDR = data;                       // Load data into data register
  TWCR = (1 << TWEN) | (1 << TWINT); // Start transmission
  while (!(TWCR & (1 << TWINT)))
    ;                   // Wait for TWINT flag to be set
  return (TWSR & 0xF8); // Return status
}

uint8_t I2C_ReadAck(void) {
  TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA); // Enable ACK
  while (!(TWCR & (1 << TWINT)))
    ;          // Wait for TWINT flag to be set
  return TWDR; // Return received data
}

uint8_t I2C_ReadNack(void) {
  TWCR = (1 << TWEN) | (1 << TWINT); // Enable NACK
  while (!(TWCR & (1 << TWINT)))
    ;          // Wait for TWINT flag to be set
  return TWDR; // Return received data
}

void I2C_WriteData(uint8_t deviceAddress, uint8_t registerAddress,uint8_t data) {
  I2C_Start();
  I2C_Write(deviceAddress
            << 1); // Write address of the I2C device with the write bit (0)
  I2C_Write(registerAddress); // Write the register address
  I2C_Write(data);            // Write the data to the register
  I2C_Stop();
}

uint8_t I2C_ReadData(uint8_t deviceAddress, uint8_t registerAddress) {
  uint8_t data;

  I2C_Start();
  I2C_Write(deviceAddress
            << 1); // Write address of the I2C device with the write bit (0)
  I2C_Write(registerAddress); // Write the register address
  I2C_Start();
  I2C_Write((deviceAddress << 1) |
            0x01); // Write address of the I2C device with the read bit (1)
  data = I2C_ReadNack(); // Read the data from the register
  I2C_Stop();

  return data;
}
