#ifndef I2C_H
#define I2C_H

#include <stdbool.h>
#include <stdint.h>

void I2C_Init(void);
bool I2C_WriteRegisters(uint8_t deviceAddress, uint8_t firstRegister,
                        const uint8_t *data, uint8_t length);
bool I2C_ReadRegisters(uint8_t deviceAddress, uint8_t firstRegister,
                       uint8_t *data, uint8_t length);

#endif
