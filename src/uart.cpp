#include "uart.h"
#include <util/delay.h>

void uart_init(void) {
    // Set baud rate to 9600
    UBRR0H = 0;
    UBRR0L = 103;

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(unsigned char data) {
    // Wait for the transmit buffer to be empty
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into the buffer, sends the data
    UDR0 = data;
}

void uart_transmit_string(const char *str) {
    // Transmit each character until null terminator is reached
    while (*str != '\0') {
        uart_transmit(*str);
        str++;
    }
}

unsigned char uart_receive(void) {
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));

    // Get and return received data from buffer
    return UDR0;
}

void uart_receive_string(char *buffer) {
    char received_char;
    uint8_t index = 0;

    // Receive characters until '\r' or '\n' is received
    while (1) {
        received_char = uart_receive();
        if (received_char == '\r' || received_char == '\n') {
            break;
        }
        buffer[index++] = received_char;

        // Check if buffer overflow occurred
        if (index >= (MAX_STRING_LENGTH - 1)) {
            break;
        }
    }

    // Null-terminate the string
    buffer[index] = '\0';
}
