#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void uart_transmit(unsigned char data);
void uart_transmit_string(const char *str);
void uart_transmit_number(uint8_t number);
uint8_t uart_receive_available(void);
unsigned char uart_receive_byte(void);
uint8_t uart_receive_overflowed(void);

#endif
