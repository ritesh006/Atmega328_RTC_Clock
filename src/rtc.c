#include "rtc.h"
#include "i2c.h"
#include "time_utils.h"

#define RTC_ADDRESS 0x68
#define RTC_SECONDS_REGISTER 0x00

RtcStatus RTC_GetTime(RtcTime *time) {
  if (time == 0) {
    return RTC_ERROR_ARGUMENT;
  }

  uint8_t registers[3];
  if (!I2C_ReadRegisters(RTC_ADDRESS, RTC_SECONDS_REGISTER,
                         registers, sizeof(registers))) {
    return RTC_ERROR_I2C;
  }

  /* On DS1307 bit 7 is CH (clock halt). DS3231 reads it as zero. */
  if ((registers[0] & 0x80) != 0) {
    return RTC_ERROR_CLOCK_STOPPED;
  }

  RtcTime decoded;
  if (!rtc_bcd_decode((uint8_t)(registers[0] & 0x7F), 59,
                      &decoded.second) ||
      !rtc_bcd_decode((uint8_t)(registers[1] & 0x7F), 59,
                      &decoded.minute) ||
      !rtc_hour_decode(registers[2], &decoded.hour)) {
    return RTC_ERROR_INVALID_TIME;
  }

  *time = decoded;
  return RTC_OK;
}

RtcStatus RTC_SetTime(uint8_t hour, uint8_t minute, uint8_t second) {
  uint8_t registers[3];
  if (!rtc_bcd_encode(second, 59, &registers[0]) ||
      !rtc_bcd_encode(minute, 59, &registers[1]) ||
      !rtc_bcd_encode(hour, 23, &registers[2])) {
    return RTC_ERROR_INVALID_TIME;
  }

  /* Write in 24-hour mode. Writing seconds also clears the DS1307 CH bit. */
  return I2C_WriteRegisters(RTC_ADDRESS, RTC_SECONDS_REGISTER,
                            registers, sizeof(registers))
             ? RTC_OK
             : RTC_ERROR_I2C;
}

RtcStatus RTC_Init(void) {
  I2C_Init();

  RtcTime current;
  return RTC_GetTime(&current);
}
