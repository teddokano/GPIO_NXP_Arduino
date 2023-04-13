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

#include <GPIO_NXP.h>

PCAL6534 gpio;

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCAL6534! *****");

  Wire.begin();
}

void loop() {
  static int count = 0;
  gpio.output(2, count++);
  delay(100);
}