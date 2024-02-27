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
#include <PORT.h>

PCAL6416A gpio;
PORT port[] = { PORT(gpio, 0),
                PORT(gpio, 1) };


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


  port[0].config(0x00);  //  Configure port0 as OUTPUT
  port[1].config(0xFF);  //  Configure port1 as INPUT

  port[0].set(PULL_UD_EN, 0x00);  //  Pull-up/down disabled
  port[1].set(PULL_UD_EN, 0xFF);  //  Pull-up/down enabled

  port[0].set(PULL_UD_SEL, 0x00);  //
  port[1].set(PULL_UD_SEL, 0xFF);  //  Pull-up selected
}
void loop() {
  int input1 = port[1];  //  Read port0 input
  port[0] = 0x55;      //  Output to port0. Higher 4 bits are input. Those are shifted to lower 4 bits

  GPIO_base::print_bin(input1);  //  Show the data on serial terminal
  Serial.println("");

  delay(100);
}
