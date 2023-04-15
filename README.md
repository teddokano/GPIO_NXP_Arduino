# GPIO_NXP_Arduino
GPIO device operation sample code for [Arduino](https://www.arduino.cc) 

> **Note**
This library works with [`I2C_device`](https://github.com/teddokano/I2C_device_Arduino) library together. Please be sure the `I2C_device` library is imported in your environment before trying to build. 

![Boards](https://github.com/teddokano/additional_files/blob/main/GPIO_NXP_Arduino/boards.jpg)  
_PCAL6416AEV-ARD (on Arduino), PCAL6534EV-ARD, PCAL6524EV-ARD and PCAL6408A-ARD (far side from left to right) : Arduino® Shield Evaluation Boards for GPIO_

## What is this?
An Arduino library for I²C GPIO-expander with sample code.  
This library provides simple API to control GPIO device IO bits

With `GPIO_NXP_Arduino` library, parallel output can be controlled by next sample code. 
```cpp
#include <GPIO_NXP.h>

PCAL6416A gpio;

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);
  Wire.begin();
  gpio.config(0, 0x00); //  Port0 is configured as output
}

void loop() {
  static int count = 0;
  gpio.output(0, count++);  //  Output to port0 
  delay(10);
}
```

## Supported devices
Type#|IO bits|Features|Interface|Evaluation board
---|---|---|---|---
[PCAL6408A](https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/low-voltage-translating-8-bit-ic-bus-smbus-i-o-expander:PCAL6408A)	|8	|Low-Voltage Translating, 8-Bit I²C-Bus/SMBus I/O Expander				|I²C Fast-mode Plus (1MHz)	|[PCAL6408A 8-Bit GPIO Arduino® Shield](https://www.nxp.com/design/development-boards/analog-toolbox/arduino-shields-solutions/pcal6408a-8-bit-gpio-arduino-shield:PCAL6408A-ARD)
[PCAL6416A](https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/low-voltage-translating-16-bit-ic-bus-smbus-i-o-expander:PCAL6416A)	|16	|Low-Voltage Translating 16-Bit I²C-Bus/SMBus I/O Expander				|I²C Fast-mode Plus (1MHz)	|[PCAL6416A 16-bit GPIO Arduino® Shield](https://www.nxp.com/design/development-boards/analog-toolbox/arduino-shields-solutions/pcal6416a-16-bit-gpio-arduino-shield-evaluation-board:PCAL6416AEV-ARD)
[PCAL6408A](https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/ic-bus-controller-and-bridge-ics/ultra-low-voltage-translating-24-bit-fm-plus-ic-bus-smbus-i-o-expander:PCAL6524)	|24	|Ultra-Low-Voltage Translating 24-Bit Fm+ I²C-Bus/SMBus I/O Expander	|I²C Fast-mode Plus (1MHz)	|[PCAL6524EV 24-Bit GPIO Arduino® Shield](https://www.nxp.com/design/development-boards/analog-toolbox/arduino-shields-solutions/pcal6524ev-24-bit-gpio-arduino-shield:PCAL6524EV-ARD)
[PCAL6408A](https://www.nxp.jp/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/ultra-low-voltage-level-translating-34-bit-ic-bus-smbus-i-o-expander:PCAL6534)	|34	|Ultra-Low-Voltage, Level Translating, 34-Bit I2C-Bus/SMBus I/O Expander|I²C Fast-mode Plus (1MHz)	|[PCAL6534EV 34-Bit GPIO Arduino® Shield](https://www.nxp.com/design/development-boards/analog-toolbox/arduino-shields-solutions/pcal6534ev-34-bit-gpio-arduino-shield:PCAL6534EV-ARD)
[PCA9554](https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/8-bit-ic-bus-and-smbus-i-o-port-with-interrupt:PCA9554_PCA9554A)<br /> **not tested**	|8	|8-Bit I²C-Bus and SMBus I/O Port with Interrupt	|I²C Fast-mode (400Hz)	|---
[PCA9555](https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/16-bit-ic-bus-and-smbus-i-o-port-with-interrupt:PCA9555)	|16	|16-Bit I²C-Bus and SMBus I/O Port with Interrupt	|I²C Fast-mode (400Hz)	|---

# Getting started

Use Library manager in Arduino IDE for easy install

# What's inside?

## Examples
Examples are provided as scketch files.

### How to use?

After library install, Choose menu on Arduino-IDE: `File`→`Examples`→`GPIO_NXP_Arduino`→ **sketch for your try**

### List of sample code

#### For PCAL6xxx GPIO Arduino® Shield evaluation boards
Sketch|Folder/Target|Feature
---|---|---
PCAL6408A_simple_OUT		|PCAL6408A	|**Simple** sample for **just output** counter value
PCAL6408A_simple_IN_OUT		|PCAL6408A	|Blink LED to show which buton pressed. Read value shown on serial terminal also
PCAL6408A_interrupt			|PCAL6408A	|IO demo with **interrupt**
PCAL6416A_simple_OUT		|PCAL6416A	|**Simple** sample for **just output** counter value
PCAL6416A_simple_IN_OUT		|PCAL6416A	|Blink LED to show which buton pressed. Read value shown on serial terminal also
PCAL6416A_interrupt			|PCAL6416A	|IO demo with **interrupt**
PCAL6524A_simple_OUT		|PCAL6524	|**Simple** sample for **just output** counter value
PCAL6524_simple_IN_OUT		|PCAL6524	|Blink LED to show which buton pressed. Read value shown on serial terminal also
PCAL6524_interrupt			|PCAL6524	|IO demo with **interrupt**
PCAL6534_simple_OUT			|PCAL6534	|**Simple** sample for **just output** counter value
PCAL6534_simple_IN_OUT		|PCAL6534	|Blink LED to show which buton pressed. Read value shown on serial terminal also
PCAL6534_interrupt			|PCAL6534	|IO demo with **interrupt**

#### For non-ARD boards
Sketch|Folder/Target|Feature
---|---|---
PCA9555_port0_OUT				|PCA9555	|**Simple** sample for **just output** counter value
PCA9555_port0_OUT_and_port1_IN	|PCA9555	|To check **output** and **input**
PCA9555_interrupt_on_port1		|PCA9555	|**Interrupt** check

# Document
For details of the library, please find descriptions in [this document](https://teddokano.github.io/GPIO_NXP_Arduino/annotated.html).

# References

## Related libraries
Library|Feature|Target devices|Required library
---|---|---|---
[GPIO_NXP_Arduino](https://github.com/teddokano/GPIO_NXP_Arduino)				|LED driver libraries				|PCAL6408A, PCAL6416A, PCAL6524, PCAL6534	|[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)
[LEDDriver_NXP_Arduino](https://github.com/teddokano/LEDDriver_NXP_Arduino)		|LED driver libraries				|PCA9955B, PCA9956B, PCA9957	|[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)
[RTC_NXP_Arduino](https://github.com/teddokano/RTC_NXP_Arduino)					|RTC driver libraries				|PCF2131, PCF85063A				|[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)
[TempSensor_NXP_Arduino](https://github.com/teddokano/TempSensor_NXP_Arduino)	|Temperature sensor libraries		|LM75B, PCT2075, P3T1085		|[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)
[I2C_device_Arduino](https://github.com/teddokano/I2C_device_Arduino)			|Base library for I²C operations	|none (can be applied as base class for all I²C targets)|---
