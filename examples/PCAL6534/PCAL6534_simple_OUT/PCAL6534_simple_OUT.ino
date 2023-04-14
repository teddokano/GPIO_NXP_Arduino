/** PCAL6534 GPIO operation sample
 *  
 *  This sample code is showing PCAL6534 GPIO operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6534:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/ultra-low-voltage-level-translating-34-bit-ic-bus-smbus-i-o-expander:PCAL6534
 */

#include <GPIO_NXP.h>

PCAL6534 gpio;

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address
  
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCAL6534! *****");

  Wire.begin();

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0x00,  // Configure port1 as OUTPUT
    0x00,  // Configure port2 as OUTPUT
    0xE0,  // Configure port3 bit 7~5 as INPUT
    0x03,  // Configure port4 bit 1 and 0 as INPUT
  };

  gpio.config(io_config_and_pull_up);  //  Port0, 1, 2 and port3 bit 4~0 are configured as output
}

void loop() {
  static int count = 0;
  gpio.output(2, count++);  //  Output to port2
  delay(100);
}
