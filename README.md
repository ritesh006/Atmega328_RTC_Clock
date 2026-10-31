# ATmega328 RTC Clock

A standalone AVR C clock for an ATmega328P/Arduino Uno and a DS1307 or DS3231 RTC. The firmware uses the AVR TWI peripheral for I2C and the ATmega328P UART for the serial display.

## Features

- Reads and displays the RTC time over UART at 9600 baud.
- Preserves the RTC's battery-backed time across MCU resets.
- Accepts a serial command to set the time in 24-hour format.
- Reads RTCs configured in either 12-hour or 24-hour register mode.
- Validates BCD/range values and reports I2C, stopped-clock, or invalid-time errors.
- Bounds I2C waits so an unplugged device or stuck bus does not hang forever.

## Hardware

- ATmega328P or Arduino Uno at 16 MHz.
- DS1307 or DS3231 breakout with a working backup cell.
- Connect RTC SDA to Uno A4/SDA and SCL to Uno A5/SCL.
- Connect grounds and use a supply level supported by the RTC board. Check board pull-ups and logic voltage before wiring.
- Serial monitor: 9600 baud, 8 data bits, no parity, 1 stop bit. Select “Newline” or “Both NL & CR” for line endings.

The project targets the Arduino Uno in PlatformIO. Do not connect a 5 V Uno directly to a 3.3 V-only RTC module.

## Build and upload

1. Open this directory in VS Code with PlatformIO installed.
2. Build the `uno` environment.
3. Connect the board and upload.
4. Open the serial monitor at 9600 baud.

No Arduino library is required by the application source. PlatformIO supplies the AVR toolchain and board support for the build.

## Set the time

The firmware does not overwrite the RTC during startup. In the serial monitor, enter a command such as:

```text
SET 14:05:00
```

Hours use 00–23; minutes and seconds use 00–59. The RTC keeps time while the ATmega resets as long as the module backup cell is healthy. Set the clock again after replacing the RTC or if the module reports that its oscillator is stopped.

## Host-side tests

The time conversion/parser tests do not require AVR hardware. With GCC installed, run from the project root:

```sh
gcc -std=c11 -Wall -Wextra -Werror -Iinclude src/time_utils.c tests/test_time_utils.c -o test_time_utils
./test_time_utils
```

The test covers BCD conversion/ranges, 12-hour and 24-hour decoding, and strict `HH:MM:SS` parsing. It does not emulate the electrical I2C bus or replace testing with the target board.

## RTC behavior

`RTC_Init()` initializes TWI and reads the RTC; it never sets the time. `RTC_SetTime()` accepts 24-hour values and writes the DS1307/DS3231 hour register in 24-hour mode. DS1307 clock-halt is cleared when a valid time is written.

I2C transactions check AVR TWI status and have bounded waits. RTC reads validate the time registers before exposing them to the display.

## License

See the project license referenced in the repository history.
