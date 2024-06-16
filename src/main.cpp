#include <Arduino.h>
#include <Wire.h>
#include "uart.h"

// pins SDA => A5
// pins SCL => A4
//      VCC
//      GND

// Define constants for DS3231 RTC
const uint8_t ONE_BYTE_DATA = 1;
const uint8_t RTC_ADDRESS = 0x68;   // Address of the RTC module
const uint8_t SEC_REGISTER = 0x00;  // Register for seconds
const uint8_t MIN_REGISTER = 0x01;  // Register for minutes
const uint8_t HOUR_REGISTER = 0x02; // Register for hours

// Initial values for time
unsigned char sec = 47;  // Seconds (0-59)
unsigned char min = 10;  // Minutes (0-59)
unsigned char hour = 10;  // Hours in 24-hour format (0-23)
unsigned char bcdHour;   // Variable to store hours in BCD format

// Function to convert decimal to BCD (Binary Coded Decimal)
unsigned char decToBcd(int val) {
  return ((val / 10 * 16) + (val % 10)); // Convert from decimal to BCD
}

// Function to convert BCD to decimal
unsigned char bcdToDec(unsigned char val) {
  return ((val / 16 * 10) + (val % 16)); // Convert from BCD to decimal
}

void setup() {
  // Initialize bcdHour with BCD value of hour
  bcdHour = decToBcd(hour);       // Convert hours to BCD format
  bcdHour |= (1 << 6);            // Set bit 6 for 12-hour mode (AM/PM)
  bcdHour |= (1 << 5);            // Set bit 5 for PM (or AM if needed)

  // Start the I2C bus as a master
  Wire.begin();                   // Start the communication with RTC

  // Set initial time on RTC
  // Set seconds register
  Wire.beginTransmission(RTC_ADDRESS);  // Begin sending data to RTC
  Wire.write(SEC_REGISTER);             // Specify the register for seconds
  Wire.write(decToBcd(sec));            // Write seconds in BCD format
  Wire.endTransmission();               // End data transmission

  // Set minutes register
  Wire.beginTransmission(RTC_ADDRESS);  // Begin sending data to RTC
  Wire.write(MIN_REGISTER);             // Specify the register for minutes
  Wire.write(decToBcd(min));            // Write minutes in BCD format
  Wire.endTransmission();               // End data transmission

  // Set hours register
  Wire.beginTransmission(RTC_ADDRESS);  // Begin sending data to RTC
  Wire.write(HOUR_REGISTER);            // Specify the register for hours
  Wire.write(bcdHour);                  // Write hours in BCD format
  Wire.endTransmission();               // End data transmission

  // Initialize serial communication for printing messages
  Serial.begin(9600);           // Initialize Serial Monitor at 9600 baud rate
  Serial.println("DS3231 Real-Time Clock Initialized"); // Print initialization message
}

void loop() {
  unsigned char sec, min, hour;          // Variables to store time values
  bool isPM;                    // Variable to indicate if it's PM or AM

  // Read seconds from RTC
  Wire.beginTransmission(RTC_ADDRESS);  // Begin sending data to RTC
  Wire.write(SEC_REGISTER);             // Specify the register for seconds
  Wire.endTransmission();               // End data transmission
  Wire.requestFrom(RTC_ADDRESS, ONE_BYTE_DATA);     // Request 1 byte of data from RTC
  if (Wire.available()) {
    sec = bcdToDec(Wire.read() & 0x7F); // (and many other RTC modules) use a control bit called the CH (Clock Halt) bit. 
    // thats why used bit masking like rtc module DS1307
  }

  // Read minutes from RTC
  Wire.beginTransmission(RTC_ADDRESS);  // Begin sending data to RTC
  Wire.write(MIN_REGISTER);             // Specify the register for minutes
  Wire.endTransmission();               // End data transmission
  Wire.requestFrom(RTC_ADDRESS, ONE_BYTE_DATA);     // Request 1 byte of data from RTC
  if (Wire.available()) {
    min = bcdToDec(Wire.read());        // Read and convert minutes from BCD to decimal
  }

  // Read hours from RTC
  Wire.beginTransmission(RTC_ADDRESS);  // Begin sending data to RTC
  Wire.write(HOUR_REGISTER);            // Specify the register for hours
  Wire.endTransmission();               // End data transmission
  Wire.requestFrom(RTC_ADDRESS, ONE_BYTE_DATA);     // Request 1 byte of data from RTC
  if (Wire.available()) {
    unsigned char rawHour = Wire.read();         // Read raw hour data
    hour = bcdToDec(rawHour & 0x1F);    // Extract and convert hours from BCD to decimal (mask out AM/PM bit)
    isPM = (rawHour & 0x20);            // Check if it's PM (bit 5 of hour register)
  }

  // Display time on Serial Monitor
  Serial.print("Time: ");
  if (hour < 10) Serial.print("0");     // Print leading zero for single digit hours
  Serial.print(hour);                   // Print hours
  Serial.print(":");
  if (min < 10) Serial.print("0");      // Print leading zero for single digit minutes
  Serial.print(min);                    // Print minutes
  Serial.print(":");
  if (sec < 10) Serial.print("0");      // Print leading zero for single digit seconds
  Serial.print(sec);                    // Print seconds
  Serial.print(isPM ? " PM" : " AM");   // Print AM/PM based on isPM flag
  Serial.println();                     // Move to next line in Serial Monitor

  delay(1000);                          // Delay for 1 second (1000 milliseconds)
}
