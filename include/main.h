#ifndef MIAN_H
#define MAIN_H
#include <util/delay.h>

// Initial values for time
const uint8_t sec = 57;  // Seconds (0-59)
const uint8_t min = 48;  // Minutes (0-59)
const uint8_t hour = 11; // Hours in 24-hour format (0-23)

// Initialize bcdHour with BCD value of hour

uint8_t decToBcd(uint8_t val);

void display_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t isPM);

uint8_t bcdToDec(uint8_t val);

uint8_t decToBcd(uint8_t val);

void write_data_to_rtc(void);

#endif