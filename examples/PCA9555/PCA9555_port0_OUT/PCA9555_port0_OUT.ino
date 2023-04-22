/** PCA9555 GPIO operation sample
 *  
 *  This sample code is showing PCA9555 GPIO operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCA9555:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/16-bit-ic-bus-and-smbus-i-o-port-with-interrupt:PCA9555
 */

#include <PCA9555.h>

PCA9555 gpio;

void setup() {
  gpio.begin();

  Serial.begin(9600);
  Serial.println("\n***** Hello, PCA9555! *****");

  Wire.begin();

  gpio.config(0, 0x00); //  Port0 is configured as output
}

void loop() {
  static int count = 0;
  gpio.output(0, count++);  //  Output to port0 
  delay(1);
}
