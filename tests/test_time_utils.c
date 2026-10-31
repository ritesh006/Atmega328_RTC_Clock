#include "time_utils.h"

#include <assert.h>
#include <stdio.h>

static void test_bcd(void) {
  uint8_t value = 0xFF;
  uint8_t decoded = 0xFF;

  assert(rtc_bcd_encode(0, 59, &value) && value == 0x00);
  assert(rtc_bcd_encode(59, 59, &value) && value == 0x59);
  assert(!rtc_bcd_encode(60, 59, &value));
  assert(rtc_bcd_decode(0x00, 59, &decoded) && decoded == 0);
  assert(rtc_bcd_decode(0x59, 59, &decoded) && decoded == 59);
  assert(!rtc_bcd_decode(0x6A, 59, &decoded));
  assert(!rtc_bcd_decode(0x60, 59, &decoded));
  assert(!rtc_bcd_decode(0x12, 59, 0));
}

static void test_hour_modes(void) {
  uint8_t hour = 0xFF;

  assert(rtc_hour_decode(0x00, &hour) && hour == 0);
  assert(rtc_hour_decode(0x23, &hour) && hour == 23);
  assert(rtc_hour_decode(0x41, &hour) && hour == 1);   /* 1 AM */
  assert(rtc_hour_decode(0x61, &hour) && hour == 13);  /* 1 PM */
  assert(rtc_hour_decode(0x52, &hour) && hour == 0);   /* 12 AM */
  assert(rtc_hour_decode(0x72, &hour) && hour == 12);  /* 12 PM */
  assert(!rtc_hour_decode(0x40, &hour));               /* invalid 12-hour zero */
  assert(!rtc_hour_decode(0x2A, &hour));               /* invalid BCD */
  assert(!rtc_hour_decode(0x23, 0));
}

static void test_time_parser(void) {
  uint8_t hour = 0, minute = 0, second = 0;

  assert(rtc_time_parse_hhmmss("00:00:00", &hour, &minute, &second));
  assert(hour == 0 && minute == 0 && second == 0);
  assert(rtc_time_parse_hhmmss("23:59:59", &hour, &minute, &second));
  assert(hour == 23 && minute == 59 && second == 59);
  assert(!rtc_time_parse_hhmmss("24:00:00", &hour, &minute, &second));
  assert(!rtc_time_parse_hhmmss("12:60:00", &hour, &minute, &second));
  assert(!rtc_time_parse_hhmmss("12:00:60", &hour, &minute, &second));
  assert(!rtc_time_parse_hhmmss("1x:00:00", &hour, &minute, &second));
  assert(!rtc_time_parse_hhmmss("12-00-00", &hour, &minute, &second));
  assert(!rtc_time_parse_hhmmss("12:00:00x", &hour, &minute, &second));
  assert(!rtc_time_parse_hhmmss(0, &hour, &minute, &second));
  assert(!rtc_time_parse_hhmmss("12:00:00", 0, &minute, &second));
}

int main(void) {
  test_bcd();
  test_hour_modes();
  test_time_parser();
  puts("All RTC time utility tests passed.");
  return 0;
}
