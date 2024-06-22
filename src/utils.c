#include "utils.h"
#include "uart.h" // Assuming uart.h defines uart_transmit_string and uart_transmit_number

// utils.c

#include "utils.h"

uint8_t decToBcd(uint8_t val) { return ((val / 10) << 4) | (val % 10); }

uint8_t bcdToDec(uint8_t val) { return ((val >> 4) * 10) + (val & 0x0F); }


void display_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t isPM) {
  uart_transmit_string("Time: ");
  uart_transmit_number(hour); // Transmit formatted hours
  uart_transmit(':');
  if (min < 10) {
    uart_transmit('0'); // Add '0' before single digit minutes
  }
  uart_transmit_number(min); // Transmit formatted minutes
  uart_transmit(':');
  if (sec < 10) {
    uart_transmit('0'); // Add '0' before single digit seconds
  }
  uart_transmit_number(sec); // Transmit formatted seconds
  uart_transmit_string(isPM ? " PM" : " AM");
  uart_transmit('\n'); // Move to the next line
}
