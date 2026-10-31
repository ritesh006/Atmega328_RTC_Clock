#include "config.h"
#include "uart.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#define UART_UBRR_VALUE ((F_CPU / (16UL * UART_BAUD)) - 1UL)
#define UART_RX_BUFFER_SIZE 32
#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1)

static volatile uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
static volatile uint8_t rxHead;
static volatile uint8_t rxTail;
static volatile uint8_t rxOverflow;

ISR(USART_RX_vect) {
  uint8_t data = UDR0;
  uint8_t next = (uint8_t)((rxHead + 1U) & UART_RX_BUFFER_MASK);

  if (next == rxTail) {
    rxOverflow = 1;
  } else {
    rxBuffer[rxHead] = data;
    rxHead = next;
  }
}

void uart_init(void) {
  rxHead = 0;
  rxTail = 0;
  rxOverflow = 0;

  UBRR0H = (uint8_t)(UART_UBRR_VALUE >> 8);
  UBRR0L = (uint8_t)UART_UBRR_VALUE;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  sei();
}

void uart_transmit(unsigned char data) {
  while ((UCSR0A & (1 << UDRE0)) == 0) {
  }
  UDR0 = data;
}

void uart_transmit_string(const char *str) {
  while (*str != '\0') {
    uart_transmit((unsigned char)*str++);
  }
}

void uart_transmit_number(uint8_t number) {
  if (number >= 100) {
    uart_transmit((unsigned char)('0' + number / 100));
  }
  if (number >= 10) {
    uart_transmit((unsigned char)('0' + (number / 10) % 10));
  }
  uart_transmit((unsigned char)('0' + number % 10));
}

uint8_t uart_receive_available(void) {
  return rxHead != rxTail;
}

unsigned char uart_receive_byte(void) {
  if (rxHead == rxTail) {
    return 0;
  }

  uint8_t tail = rxTail;
  unsigned char value = rxBuffer[tail];
  rxTail = (uint8_t)((tail + 1U) & UART_RX_BUFFER_MASK);
  return value;
}

uint8_t uart_receive_overflowed(void) {
  uint8_t overflowed = rxOverflow;
  rxOverflow = 0;
  return overflowed;
}
