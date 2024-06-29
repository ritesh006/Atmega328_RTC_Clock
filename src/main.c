#include "config.h"
#include "rtc.h"
#include "uart.h"
#include "utils.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void) {

  uint8_t hours, minutes, seconds, isPM;
  // set_port_output();
  // Initialize RTC and UART
  RTC_Init();
  uart_init();

  // Set initial time on RTC
  RTC_SetTime(12, 0, 0); // Set initial time to 12:00:00 PM

  // Main loop
  while (1)
  {
    // Read time from RTC
    RTC_GetTime(&hours, &minutes, &seconds, &isPM);

    // Display time over UART
    display_time(hours, minutes, seconds, isPM);
    
    // Delay for 1 second
    _delay_ms(1000);
  }

  return 0;
}
