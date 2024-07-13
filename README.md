# ATmega328 RTC Clock Project

This project demonstrates how to create a simple clock using an ATmega328 microcontroller interfaced with RTC modules DS1307 and DS3231. 
The project does not rely on any Arduino libraries, but rather includes custom implementations for I2C and UART communication.

## Features

- Real-time clock functionality with DS1307 and DS3231 RTC modules
- Custom I2C and UART libraries for efficient communication
- Displays the current time over UART
- Developed for the ATmega328 microcontroller

## Components Used

- ATmega328 microcontroller
- DS1307 or DS3231 RTC module
- Arduino IDE or VS Code with PlatformIO

## Hardware Setup

### RTC Module Connection:

- Connect the SDA pin of the RTC module to A4 on the Arduino.
- Connect the SCL pin of the RTC module to A5 on the Arduino.
- Connect VCC and GND pins to the appropriate power and ground pins.

## Software Setup
### Clone the Repository:

```bash
git clone https://github.com/ritesh006/Atmega328_RTC_Clock.git

### Open the Project:

Open the project in VS Code with PlatformIO or Arduino IDE.

### Upload the Code:

1. Select the appropriate board and COM port.
2. Upload the code to the Arduino.

### Monitor Output:

Open the Serial Monitor at a baud rate of 9600 to view the current time.

## Custom Libraries

- **I2C Library**: Handles communication with the RTC modules, including initialization, read, and write operations.
- **UART Library**: Manages serial communication, including initialization, data transmission, and reception.

## Contributing

Contributions are welcome! Please open an issue or create a pull request for any improvements or suggestions.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
