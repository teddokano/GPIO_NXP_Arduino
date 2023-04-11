/** PCAL6534 GPIO operation sample
 *  
 *  This sample code is showing PCAL6534 GPIO operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6534:
 *    https://www.nxp.jp/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/ultra-low-voltage-level-translating-34-bit-ic-bus-smbus-i-o-expander:PCAL6534
 */

#include "GPIO_NXP.h"

PCAL6534 gpio;

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCAL6534! *****");

  Wire.begin();

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure as OUTPUT
    0x00,  // Configure as OUTPUT
    0x00,  // Configure as OUTPUT
    0xE0,  // Configure as INPUT for bit 7~5
    0x03,  // Configure as INPUT for bit 1 and 0
  };
  gpio.config(io_config_and_pull_up);

  gpio.write_r8(PCAL6534::Pull_up_pull_down_enable_register_port_3, 0xE0);
  gpio.write_r8(PCAL6534::Pull_up_pull_down_enable_register_port_4, 0x03);
  gpio.write_r8(PCAL6534::Pull_up_pull_down_selection_register_port_3, 0xE0);
  gpio.write_r8(PCAL6534::Pull_up_pull_down_selection_register_port_4, 0x03);
}
void loop() {
#if 0
  static int count = 0;
  gpio.output(2, count++);
#endif
  int input3 = gpio.input(3);
  int input4 = gpio.input(4);
  gpio.output(2, (input3 & 0xFC) | input4);


  Serial.print(" ");
  Serial.print(input3, HEX);
  Serial.print(" ");
  Serial.print(input4, HEX);
  Serial.println("");

  delay(100);
}
