#include "utils.h"
#include "uart.h"

static void transmit_two_digits(uint8_t value) {
  if (value < 10) {
    uart_transmit('0');
  }
  uart_transmit_number(value);
}

void display_time(uint8_t hour, uint8_t minute, uint8_t second) {
  uart_transmit_string("Time: ");
  transmit_two_digits(hour);
  uart_transmit(':');
  transmit_two_digits(minute);
  uart_transmit(':');
  transmit_two_digits(second);
  uart_transmit_string("\r\n");
}
