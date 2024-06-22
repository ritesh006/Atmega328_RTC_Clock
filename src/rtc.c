#include "rtc.h"
#include "i2c.h"
#include "utils.h"

void RTC_Init(void) {
  I2C_Init(); // Start the communication with RTC
}

void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  uint8_t bcdHours = decToBcd(hours);
  bcdHours |= (1 << 6); // Set bit 6 for 12-hour mode (AM/PM)
  if (hours >= 12)
    bcdHours |= (1 << 5); // Set bit 5 for PM

  I2C_WriteData(RTC_ADDRESS, SEC_REGISTER, decToBcd(seconds)); // Set initial seconds
  I2C_WriteData(RTC_ADDRESS, MIN_REGISTER, decToBcd(minutes));                    // Set initial minutes
  I2C_WriteData(RTC_ADDRESS, HOUR_REGISTER, bcdHours); // Set initial hour
}

// void RTC_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds, uint8_t *isPM) 
// {
//   uint8_t rawHours;

//   *seconds = bcdToDec(I2C_ReadData(RTC_ADDRESS, SEC_REGISTER) & 0x7F);
//   *minutes = bcdToDec(I2C_ReadData(RTC_ADDRESS, MIN_REGISTER) & 0x7F);
//   rawHours = I2C_ReadData(RTC_ADDRESS, HOUR_REGISTER);

//   *isPM = (rawHours & (1 << 5)) ? 1 : 0; // Check if it's PM
//   *hours = bcdToDec(rawHours &0x1F); // Extract and convert hours from BCD to decimal
// }
