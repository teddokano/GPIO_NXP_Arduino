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
  Serial.println("\n***** Hello, PCA9955B! *****");

  Wire.begin();

  uint8_t config_and_int_setting[] = {
    0x00,  // Configure as OUTPUT
    0x00,  // Configure as OUTPUT
    0x00,  // Configure as OUTPUT
    0xE0,  // Configure as INPUT for bit 7~5
    0x03,  // Configure as INPUT for bit 1 and 0
  };

  gpio.config( config_and_int_setting );
}
void loop() {
  static int count = 0;

  gpio.output(2, count++);
  delay(100);
}
