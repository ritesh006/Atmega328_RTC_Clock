#ifndef RTC_H
#define RTC_H

#include <stdint.h>

typedef struct {
  uint8_t hour;   /* 0-23 */
  uint8_t minute; /* 0-59 */
  uint8_t second; /* 0-59 */
} RtcTime;

typedef enum {
  RTC_OK = 0,
  RTC_ERROR_I2C,
  RTC_ERROR_CLOCK_STOPPED,
  RTC_ERROR_INVALID_TIME,
  RTC_ERROR_ARGUMENT
} RtcStatus;

/* Initializes the I2C peripheral and checks that the RTC contains valid time.
 * It never changes the time stored in the RTC.
 */
RtcStatus RTC_Init(void);
RtcStatus RTC_SetTime(uint8_t hour, uint8_t minute, uint8_t second);
RtcStatus RTC_GetTime(RtcTime *time);

#endif
