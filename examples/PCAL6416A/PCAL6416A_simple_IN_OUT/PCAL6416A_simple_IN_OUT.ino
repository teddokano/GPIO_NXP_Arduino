/** PCAL6416A GPIO operation sample
 *  
 *  This sample code is showing PCAL6416A GPIO operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6416A:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/low-voltage-translating-16-bit-ic-bus-smbus-i-o-expander:PCAL6416A
 */

#include <PCAL6416A.h>

PCAL6416A gpio;

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address
  
  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  Serial.println("\n***** Hello, PCAL6416A! *****");

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0xFF,  // Configure port1 as INTPUT
  };

  gpio.config(io_config_and_pull_up);                   //  Port0 as OUTPUT, port1 as INPUT
  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);   //  Port1 Pull-up/down are enabled
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);  //  Port1 Pull-up/down are as pull-up
}
void loop() {
  int input1 = gpio.input(1); //  Read port0 input
  gpio.output(0, input1);     //  Output to port0

//  GPIO_base::print_bin(input1); //  Show the data on serial terminal
  Serial.println("");

  delay(100);
}
