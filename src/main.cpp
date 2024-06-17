#include "uart.h"
#include <util/delay.h>
#include <Wire.h>
#include <Arduino.h>
#include "main.h"
// pins SDA => A5
// pins SCL => A4
//      VCC
//      GND


uint8_t bcdHour; // Convert hours to BCD format


void setup()
{
  bcdHour = decToBcd(hour);
  bcdHour |= (1 << 6); // Set bit 6 for 12-hour mode (AM/PM)
  bcdHour |= (1 << 5); // Set bit 5 for PM (or AM if needed)
  // Start the I2C bus as a master
  Wire.begin(); // Start the communication with RTC

  // Set initial time on RTC
  // Set seconds register
  Wire.beginTransmission(RTC_ADDRESS); // Begin sending data to RTC
  Wire.write(SEC_REGISTER);            // Specify the register for seconds
  Wire.write(decToBcd(sec));           // Write seconds in BCD format
  Wire.endTransmission();              // End data transmission

  // Set minutes register
  Wire.beginTransmission(RTC_ADDRESS); // Begin sending data to RTC
  Wire.write(MIN_REGISTER);            // Specify the register for minutes
  Wire.write(decToBcd(min));           // Write minutes in BCD format
  Wire.endTransmission();              // End data transmission

  // Set hours register
  Wire.beginTransmission(RTC_ADDRESS); // Begin sending data to RTC
  Wire.write(HOUR_REGISTER);           // Specify the register for hours
  Wire.write(bcdHour);                 // Write hours in BCD format
  Wire.endTransmission();              // End data transmission

  // Initialize serial communication for printing messages
  uart_init();                                                  // Initialize Serial Monitor at 9600 baud rate
  uart_transmit_string("DS3231 Real-Time Clock Initialized\n"); // Print initialization message
}

void loop()
{
  uint8_t sec, min, hour; // Variables to store time values
  bool isPM;              // Variable to indicate if it's PM or AM

  // Read seconds from RTC
  Wire.beginTransmission(RTC_ADDRESS);          // Begin sending data to RTC
  Wire.write(SEC_REGISTER);                     // Specify the register for seconds
  Wire.endTransmission();                       // End data transmission
  Wire.requestFrom(RTC_ADDRESS, ONE_BYTE_DATA); // Request 1 byte of data from RTC
  if (Wire.available())
  {
    sec = bcdToDec(Wire.read() & 0x7F); // (and many other RTC modules) use a control bit called the CH (Clock Halt) bit.
    // thats why used bit masking like rtc module DS1307
  }

  // Read minutes from RTC
  Wire.beginTransmission(RTC_ADDRESS); // Begin sending data to RTC
  Wire.write(MIN_REGISTER);            // Specify the register for minutes
  Wire.endTransmission();              // End data transmission
  Wire.requestFrom(RTC_ADDRESS, ONE_BYTE_DATA); // Request 1 byte of data from RTC
  if (Wire.available())
  {
    min = bcdToDec(Wire.read()); // Read and convert minutes from BCD to decimal
  }

  // Read hours from RTC
  Wire.beginTransmission(RTC_ADDRESS); // Begin sending data to RTC
  Wire.write(HOUR_REGISTER);           // Specify the register for hours
  Wire.endTransmission();              // End data transmission
  Wire.requestFrom(RTC_ADDRESS, ONE_BYTE_DATA); // Request 1 byte of data from RTC
  if (Wire.available())
  {
    unsigned char rawHour = Wire.read(); // Read raw hour data
    hour = bcdToDec(rawHour & 0x1F);     // Extract and convert hours from BCD to
                                         // decimal (mask out AM/PM bit)
    isPM = (rawHour & 0x20);             // Check if it's PM (bit 5 of hour register)
  }

  // Display time on Serial Monitor
  display_time(hour, min, sec, isPM);

  _delay_ms(1000); // Delay for 1 second (1000 milliseconds)
}

