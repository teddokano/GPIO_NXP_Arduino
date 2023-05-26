/** PCAL6524 GPIO operation sample
 *  
 *  This sample code is showing PCAL6524 GPIO operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6524:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/ic-bus-controller-and-bridge-ics/ultra-low-voltage-translating-24-bit-fm-plus-ic-bus-smbus-i-o-expander:PCAL6524
 */

#include <PCAL6524.h>

PCAL6524 gpio;

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address
  
  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  Serial.println("\n***** Hello, PCAL6524! *****");

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0x00,  // Configure port1 as OUTPUT
    0xF0,  // Configure port2 bit 7~4 as INTPUT
  };

  gpio.config(io_config_and_pull_up);                   //  Port0, 1 and port2 bit 3~0 are configured as output
  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);   //  Pull-up/down enabled for port2 bit 7~4
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);  //  Pull-up selected for port2 bit 7~4
}

void loop() {
  int input2 = gpio.input(2);   //  Read port0 input
  gpio.output(2, input2 >> 4);  //  Output to port2. Higher 4 bits are input. Those are shifted to lower 4 bits

  GPIO_base::print_bin(input2); //  Show the data on serial terminal
  Serial.println("");

  delay(100);
}
