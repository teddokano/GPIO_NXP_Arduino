/** PCAL6408A GPIO operation sample
 *  
 *  This sample code is showing PCAL6408A GPIO operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6408A:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/low-voltage-translating-8-bit-ic-bus-smbus-i-o-expander:PCAL6408A
 */

#include <GPIO_NXP.h>

PCAL6408A gpio;

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCAL6408A! *****");

  Wire.begin();
  I2C_device::scan();

  gpio.config(0, 0xF0); //  Port0 bit3~0 are configured as output
}

void loop() {
  static int count = 0;
  gpio.output(0, count++);  //  Output to port0 
  delay(100);
}
