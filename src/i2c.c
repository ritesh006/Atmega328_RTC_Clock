#include "i2c.h"
#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define I2C_WAIT_LIMIT 60000U

static bool wait_for_interrupt(void) {
  uint16_t remaining = I2C_WAIT_LIMIT;
  while ((TWCR & (1 << TWINT)) == 0 && remaining > 0) {
    _delay_us(1);
    --remaining;
  }
  return (TWCR & (1 << TWINT)) != 0;
}

static bool wait_for_stop(void) {
  uint16_t remaining = I2C_WAIT_LIMIT;
  while ((TWCR & (1 << TWSTO)) != 0 && remaining > 0) {
    _delay_us(1);
    --remaining;
  }
  return (TWCR & (1 << TWSTO)) == 0;
}

static bool start_condition(void) {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  if (!wait_for_interrupt()) {
    return false;
  }
  return (TWSR & 0xF8) == TW_START || (TWSR & 0xF8) == TW_REP_START;
}

static bool stop_condition(void) {
  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
  return wait_for_stop();
}

static bool write_byte(uint8_t value, uint8_t expectedStatus) {
  TWDR = value;
  TWCR = (1 << TWINT) | (1 << TWEN);
  if (!wait_for_interrupt()) {
    return false;
  }
  return (TWSR & 0xF8) == expectedStatus;
}

static bool read_byte(uint8_t *value, bool acknowledge) {
  if (value == 0) {
    return false;
  }

  TWCR = (1 << TWINT) | (1 << TWEN) |
         (acknowledge ? (1 << TWEA) : 0);
  if (!wait_for_interrupt()) {
    return false;
  }

  uint8_t expectedStatus = acknowledge ? TW_MR_DATA_ACK : TW_MR_DATA_NACK;
  if ((TWSR & 0xF8) != expectedStatus) {
    return false;
  }

  *value = TWDR;
  return true;
}

void I2C_Init(void) {
  TWSR = 0;
  TWBR = (uint8_t)((F_CPU / SCL_CLOCK - 16UL) / 2UL);
  TWCR = (1 << TWEN);
}

bool I2C_WriteRegisters(uint8_t deviceAddress, uint8_t firstRegister,
                        const uint8_t *data, uint8_t length) {
  bool success = false;

  if (deviceAddress > 0x7F || data == 0 || length == 0) {
    return false;
  }

  if (!start_condition() ||
      !write_byte((uint8_t)(deviceAddress << 1), TW_MT_SLA_ACK) ||
      !write_byte(firstRegister, TW_MT_DATA_ACK)) {
    goto cleanup;
  }

  for (uint8_t i = 0; i < length; ++i) {
    if (!write_byte(data[i], TW_MT_DATA_ACK)) {
      goto cleanup;
    }
  }
  success = true;

cleanup:
  if (!stop_condition()) {
    success = false;
  }
  return success;
}

bool I2C_ReadRegisters(uint8_t deviceAddress, uint8_t firstRegister,
                       uint8_t *data, uint8_t length) {
  bool success = false;

  if (deviceAddress > 0x7F || data == 0 || length == 0) {
    return false;
  }

  if (!start_condition() ||
      !write_byte((uint8_t)(deviceAddress << 1), TW_MT_SLA_ACK) ||
      !write_byte(firstRegister, TW_MT_DATA_ACK) ||
      !start_condition() ||
      !write_byte((uint8_t)((deviceAddress << 1) | 1), TW_MR_SLA_ACK)) {
    goto cleanup;
  }

  for (uint8_t i = 0; i < length; ++i) {
    if (!read_byte(&data[i], i + 1 < length)) {
      goto cleanup;
    }
  }
  success = true;

cleanup:
  if (!stop_condition()) {
    success = false;
  }
  return success;
}
