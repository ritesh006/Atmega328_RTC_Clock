#include "main.h"
#include "uart.h"

uint8_t decToBcd(uint8_t val)
{
  return ((val / 10 * 16) + (val % 10)); // Convert from decimal to BCD
}

// Function to convert BCD to decimal
uint8_t bcdToDec(uint8_t val)
{
  return ((val / 16 * 10) + (val % 16)); // Convert from BCD to decimal
}

void display_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t isPM) {
  uart_transmit_string("Time: ");
  uart_transmit_number(hour); // Transmit formatted hours
  uart_transmit(':');
  uart_transmit_number(min); // Transmit formatted minutes
  uart_transmit(':');
  uart_transmit_number(sec); // Transmit formatted seconds
  uart_transmit_string(isPM ? " PM" : " AM");
  uart_transmit('\n'); // Move to the next line
}


