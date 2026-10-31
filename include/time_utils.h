#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <stdbool.h>
#include <stdint.h>

bool rtc_bcd_encode(uint8_t value, uint8_t maximum, uint8_t *encoded);
bool rtc_bcd_decode(uint8_t encoded, uint8_t maximum, uint8_t *value);
bool rtc_hour_decode(uint8_t hourRegister, uint8_t *hour24);
bool rtc_time_parse_hhmmss(const char *text, uint8_t *hour,
                           uint8_t *minute, uint8_t *second);

#endif
