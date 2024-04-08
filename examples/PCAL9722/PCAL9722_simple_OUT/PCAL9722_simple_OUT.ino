/** PCAL9722 GPIO operation sample
 *  
 *  This sample code is showing PCAL9722 GPIO operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL9722:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/22-bit-spi-i-o-expander-with-agile-i-o-features:PCAL9722
 */

#include <PCAL9722.h>

PCAL9722 gpio;

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address

  Serial.begin(9600);
  while (!Serial)
    ;

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4
  
  Serial.println("\n***** Hello, PCAL9722! *****");

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0x00,  // Configure port1 as OUTPUT
    0x3F,  // Configure port2 bit 5~0 as INTPUT
  };

  gpio.config(io_config_and_pull_up);  //  Port0, 1 and port2 bit 3~0 are configured as output
}

void loop() {
  static int count = 0;
  gpio.output(0, count++);  //  Output to port2
  gpio.output(1, 1 << (count & 0x7));  //  Output to port2
  delay(50);
}
