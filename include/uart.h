#ifndef UART_H
#define UART_H
#include <avr/io.h>
void uart_init(void);
void uart_transmit(unsigned char data);
void uart_transmit_string(const char *str);
unsigned char uart_receive(void);
void uart_receive_string(char *buffer);
void uart_transmit_number(uint8_t number);

#endif // UART_H
