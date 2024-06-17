#ifndef MIAN_H
#define MAIN_H
#include <util/delay.h>


// Define constants for DS3231 RTC
const uint8_t ONE_BYTE_DATA = 1;
const uint8_t RTC_ADDRESS = 0x68;   // Address of the RTC module
const uint8_t SEC_REGISTER = 0x00;  // Register for seconds
const uint8_t MIN_REGISTER = 0x01;  // Register for minutes
const uint8_t HOUR_REGISTER = 0x02; // Register for hours

// Initial values for time
const uint8_t sec = 57;  // Seconds (0-59)
const uint8_t min = 59;  // Minutes (0-59)
const uint8_t hour = 11; // Hours in 24-hour format (0-23)

// Initialize bcdHour with BCD value of hour

uint8_t decToBcd(uint8_t val);

void display_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t isPM);

uint8_t bcdToDec(uint8_t val);

uint8_t decToBcd(uint8_t val);

void write_data_to_rtc(void);

#endif