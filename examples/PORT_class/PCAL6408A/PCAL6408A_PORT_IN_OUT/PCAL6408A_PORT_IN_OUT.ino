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

#include <PCAL6408A.h>
#include <PORT.h>

PCAL6408A gpio;
PORT port(gpio, 0);

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address

  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  Serial.println("\n***** Hello, PCAL6408A! *****");

  constexpr uint8_t io_config_and_pull_up = 0xF0;

  port.config(io_config_and_pull_up);                   //  Port0 bit7~4 are INPUT, bit3~0 are are OUTPUT
  port.set(PULL_UD_EN, io_config_and_pull_up);   //  Pull-up/down in port0 bit7~4 are enabled
  port.set(PULL_UD_SEL, io_config_and_pull_up);  //  Pull-up/down in port0 bit7~4 are as pull-up
}
void loop() {
  int input0 = port;   //  Read port0 input
  port = input0 >> 4;  //  Output to port0. Higher 4 bits are input. Those are shifted to lower 4 bits

  GPIO_base::print_bin(input0);  //  Show the data on serial terminal
  Serial.println("");

  delay(100);
}
