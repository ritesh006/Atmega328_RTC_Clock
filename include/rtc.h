#ifndef RTC_H
#define RTC_H

#include <stdint.h>

#define RTC_ADDRESS 0x68
#define SEC_REGISTER 0x00
#define MIN_REGISTER 0x01
#define HOUR_REGISTER 0x02

void RTC_Init(void);
void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
void RTC_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds, uint8_t *isPM);

#endif // RTC_H
