#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

#define MAX_STRING_LENGTH 64

void uart_init(void);
void uart_transmit(unsigned char data);
void uart_transmit_string(const char *str);
unsigned char uart_receive(void);
void uart_receive_string(char *buffer);

#endif /* UART_H_ */
