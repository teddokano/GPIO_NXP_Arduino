/** PCAL6534 GPIO operation sample
 *  
 *  This sample code is showing PCAL6534 GPIO operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6534:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/ultra-low-voltage-level-translating-34-bit-ic-bus-smbus-i-o-expander:PCAL6534
 */

#include <PCAL6534.h>

#include <PCAL6534.h>
#include <PORT.h>

PCAL6534 gpio;
PORT port[] = { PORT(gpio, 0),
                PORT(gpio, 1),
                PORT(gpio, 2),
                PORT(gpio, 3),
                PORT(gpio, 4) };

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address

  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  Serial.println("\n***** Hello, PCAL6534! *****");

  port[0].config(0x00);  //  Configure port0 as OUTPUT
  port[1].config(0x00);  //  Configure port1 as OUTPUT
  port[2].config(0x00);  //  Configure port2 as OUTPUT
  port[3].config(0xE0);  //  Configure port3 bit 7~5 as INPUT
  port[4].config(0x03);  //  Configure port4 bit 1 and 0 as INPUT

  port[0].set(PULL_UD_EN, 0x00);  //  Pull-up/down disabled
  port[1].set(PULL_UD_EN, 0x00);  //  Pull-up/down disabled
  port[2].set(PULL_UD_EN, 0x00);  //  Pull-up/down disabled
  port[3].set(PULL_UD_EN, 0xE0);  //  Pull-up/down enabled for port3 bit 7~5
  port[4].set(PULL_UD_EN, 0x03);  //  Pull-up/down enabled for port4 bit 1 and 0

  port[0].set(PULL_UD_SEL, 0x00);  //
  port[1].set(PULL_UD_SEL, 0x00);  //
  port[2].set(PULL_UD_SEL, 0x00);  //
  port[3].set(PULL_UD_SEL, 0xE0);  //  Pull-up selected for port3 bit 7~5
  port[4].set(PULL_UD_SEL, 0x03);  //  Pull-up selected for port4 bit 1 and 0
}

void loop() {
  int input3 = port[3];  //  Read port3 input
  int input4 = port[4];  //  Read port4 input
  int output2 = (input3 & 0xFC) | input4;
  port[2] = output2;  //  Output to port2

  GPIO_base::print_bin(output2);  //  Show the data on serial terminal
  Serial.println("");
  delay(100);
}
