#include "time_utils.h"

#include <stddef.h>
#include <string.h>

bool rtc_bcd_encode(uint8_t value, uint8_t maximum, uint8_t *encoded) {
  if (encoded == 0 || value > maximum) {
    return false;
  }

  *encoded = (uint8_t)(((value / 10U) << 4) | (value % 10U));
  return true;
}

bool rtc_bcd_decode(uint8_t encoded, uint8_t maximum, uint8_t *value) {
  uint8_t high = (uint8_t)(encoded >> 4);
  uint8_t low = (uint8_t)(encoded & 0x0F);

  if (value == 0 || high > 9 || low > 9) {
    return false;
  }

  uint8_t decoded = (uint8_t)(high * 10U + low);
  if (decoded > maximum) {
    return false;
  }

  *value = decoded;
  return true;
}

bool rtc_hour_decode(uint8_t hourRegister, uint8_t *hour24) {
  if (hour24 == 0) {
    return false;
  }

  if ((hourRegister & 0x40) != 0) {
    uint8_t hour12;
    if (!rtc_bcd_decode((uint8_t)(hourRegister & 0x1F), 12, &hour12) ||
        hour12 == 0) {
      return false;
    }

    *hour24 = (uint8_t)((hour12 % 12U) +
                        ((hourRegister & 0x20) != 0 ? 12U : 0U));
    return true;
  }

  return rtc_bcd_decode((uint8_t)(hourRegister & 0x3F), 23, hour24);
}

static bool is_digit(char value) {
  return value >= '0' && value <= '9';
}

static uint8_t two_digits(const char *text) {
  return (uint8_t)((text[0] - '0') * 10 + (text[1] - '0'));
}

bool rtc_time_parse_hhmmss(const char *text, uint8_t *hour,
                           uint8_t *minute, uint8_t *second) {
  if (text == 0 || hour == 0 || minute == 0 || second == 0 ||
      strlen(text) != 8 || text[2] != ':' || text[5] != ':' ||
      !is_digit(text[0]) || !is_digit(text[1]) ||
      !is_digit(text[3]) || !is_digit(text[4]) ||
      !is_digit(text[6]) || !is_digit(text[7])) {
    return false;
  }

  uint8_t parsedHour = two_digits(text);
  uint8_t parsedMinute = two_digits(text + 3);
  uint8_t parsedSecond = two_digits(text + 6);
  if (parsedHour > 23 || parsedMinute > 59 || parsedSecond > 59) {
    return false;
  }

  *hour = parsedHour;
  *minute = parsedMinute;
  *second = parsedSecond;
  return true;
}
