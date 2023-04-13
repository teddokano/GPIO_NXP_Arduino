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

  constexpr uint8_t io_config_and_pull_up = 0xF0;

  gpio.write_port(CONFIG, io_config_and_pull_up);       //  Port0 bit7~4 are INPUT, bit3~0 are are OUTPUT
  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);   //  Pull-up/down in port0 bit7~4 are enabled
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);  //  Pull-up/down in port0 bit7~4 are as pull-up
}
void loop() {
  int input0 = gpio.input(0);   //  Read port0 input
  gpio.output(0, input0 >> 4);  //  Output to port0. Higher 4 bits are input. Those are shifted to lower 4 bits

  GPIO_base::print_bin(input0); //  Show the data on serial terminal
  Serial.println("");

  delay(100);
}
