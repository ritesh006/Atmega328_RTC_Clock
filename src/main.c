#include "rtc.h"
#include "time_utils.h"
#include "uart.h"
#include "utils.h"

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#define INPUT_BUFFER_SIZE 16
#define LOOP_DELAY_MS 10
#define DISPLAY_INTERVAL_MS 1000

static char inputBuffer[INPUT_BUFFER_SIZE];
static uint8_t inputLength;

static void print_status(RtcStatus status) {
  switch (status) {
  case RTC_OK:
    break;
  case RTC_ERROR_I2C:
    uart_transmit_string("RTC communication error. Check power and I2C wiring.\r\n");
    break;
  case RTC_ERROR_CLOCK_STOPPED:
    uart_transmit_string("RTC clock is stopped. Set it with: SET HH:MM:SS\r\n");
    break;
  case RTC_ERROR_INVALID_TIME:
    uart_transmit_string("RTC contains invalid time data. Set it with: SET HH:MM:SS\r\n");
    break;
  case RTC_ERROR_ARGUMENT:
  default:
    uart_transmit_string("RTC error.\r\n");
    break;
  }
}

static void process_command(void) {
  uint8_t hour, minute, second;

  if (strncmp(inputBuffer, "SET ", 4) != 0 ||
      !rtc_time_parse_hhmmss(inputBuffer + 4, &hour, &minute, &second)) {
    uart_transmit_string("Use: SET HH:MM:SS (24-hour time)\r\n");
    return;
  }

  RtcStatus status = RTC_SetTime(hour, minute, second);
  if (status == RTC_OK) {
    uart_transmit_string("Time set.\r\n");
    display_time(hour, minute, second);
  } else {
    print_status(status);
  }
}

static void handle_serial_input(void) {
  if (uart_receive_overflowed()) {
    inputLength = 0;
    uart_transmit_string("Serial input overflow. Try the command again.\r\n");
  }

  while (uart_receive_available()) {
    unsigned char ch = uart_receive_byte();

    if (ch == '\r' || ch == '\n') {
      if (inputLength > 0) {
        inputBuffer[inputLength] = '\0';
        process_command();
        inputLength = 0;
      }
    } else if (inputLength < INPUT_BUFFER_SIZE - 1) {
      inputBuffer[inputLength++] = (char)ch;
    } else {
      inputLength = 0;
      uart_transmit_string("Input too long. Use: SET HH:MM:SS\r\n");
    }
  }
}

int main(void) {
  uart_init();
  uart_transmit_string("\r\nATmega328 RTC Clock (9600 baud)\r\n");
  uart_transmit_string("Set time with: SET HH:MM:SS (24-hour time)\r\n");

  RtcStatus lastStatus = RTC_Init();
  if (lastStatus != RTC_OK) {
    print_status(lastStatus);
  }

  uint16_t elapsed = 0;
  while (1) {
    handle_serial_input();

    if (elapsed >= DISPLAY_INTERVAL_MS) {
      RtcTime current;
      RtcStatus status = RTC_GetTime(&current);
      if (status == RTC_OK) {
        display_time(current.hour, current.minute, current.second);
      } else if (status != lastStatus) {
        print_status(status);
      }
      lastStatus = status;
      elapsed = 0;
    }

    _delay_ms(LOOP_DELAY_MS);
    elapsed += LOOP_DELAY_MS;
  }
}
