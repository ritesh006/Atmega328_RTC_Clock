#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>


void uart_transmit_number(uint8_t number);
uint8_t decToBcd(uint8_t val);
uint8_t bcdToDec(uint8_t val);
void display_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t isPM);

#endif /* UTILS_H_ */
