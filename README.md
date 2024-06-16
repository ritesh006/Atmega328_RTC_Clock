# Atmega328_RTC_Clock

This Arduino project demonstrates how to create a simple clock using an Arduino board without using an RTC (Real-Time Clock) library. Instead, it utilizes the Arduino's internal `millis()` function for timekeeping.

## Used lib
#include <Arduino.h>
#include <Wire.h>

pins SDA => A5
pins SCL => A4
     VCC
     GND
## Overview

This sketch implements a basic clock that displays the current time on the Serial Monitor. It uses the Arduino `millis()` function to track elapsed time since the sketch started, which allows it to maintain timekeeping functionality without an external RTC module.

## Getting Started

### Prerequisites

- VS code with platform.io IDE installed on your computer.

- Arduino board (e.g., Arduino Uno, Arduino Nano).

- USB cable for connecting Arduino to your computer.

### Installing and Running

1\. **Clone or download the repository to your local machine:**

   ```bash

   git clone https://github.com/ritesh006/Atmega328_RTC_Clock.git
1.  Open the Arduino IDE.
2.  Navigate to File -> Open and select the `ArduinoIDE_clock_Withou_RTC_lib.ino` file from the cloned repository.
3.  Connect your Arduino board to your computer using a USB cable.
4.  Upload the sketch to your Arduino board (Sketch -> Upload).
5.  Open the Serial Monitor (Tools -> Serial Monitor). Ensure the baud rate is set to 9600 to view the clock output.

Usage
-----

-   The clock starts running as soon as the sketch is uploaded to the Arduino board.
-   The Serial Monitor displays the current time in hours, minutes, and seconds.
-   Time increments every second.

Code Explanation
----------------

-   `setup()`: Initializes Serial communication and sets up initial parameters.
-   `loop()`: Continuously updates and displays the current time using the `millis()` function.
-   The sketch calculates hours, minutes, and seconds based on the elapsed time since the sketch started.

Contributing
------------

Contributions are welcome! If you find any issues or have suggestions for improvements, please feel free to open an issue or create a pull request.

License
-------

This project is licensed under the MIT License. See the LICENSE file for details.