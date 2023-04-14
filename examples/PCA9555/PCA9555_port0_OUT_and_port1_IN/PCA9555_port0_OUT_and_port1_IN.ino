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

#include <GPIO_NXP.h>

PCA9555 gpio;

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCA9555! *****");

  Wire.begin();

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0xFF,  // Configure port1 as INTPUT
  };

  gpio.config(io_config_and_pull_up);  //  Port0 as OUTPUT, port1 as INPUT
}
void loop() {
  static int count = 0;
  gpio.output(0, count);        //  Output to port0

  int input1 = gpio.input(1);   //  Read port1 input

  GPIO_base::print_bin(count);  //  Show the data on serial terminal
  GPIO_base::print_bin(input1); //  Show the data on serial terminal
  Serial.println("");

  count++;
  delay(100);
}
