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
#include <PORT.h>

PCAL6524 gpio;
PORT port[] = { PORT(gpio, 0),
                PORT(gpio, 1),
                PORT(gpio, 2) };

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address

  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  Serial.println("\n***** Hello, PCAL6524! *****");

  port[0].config(0x00);  //  Configure port0 as OUTPUT
  port[1].config(0x00);  //  Configure port1 as OUTPUT
  port[2].config(0xF0);  //  Configure port2 bit 7~4 as INTPUT

  port[0].set(PULL_UD_EN, 0x00);  //  Pull-up/down disabled
  port[1].set(PULL_UD_EN, 0x00);  //  Pull-up/down disabled
  port[2].set(PULL_UD_EN, 0xF0);  //  Pull-up/down enabled for port2 bit 7~4

  port[0].set(PULL_UD_SEL, 0x00);  //
  port[1].set(PULL_UD_SEL, 0x00);  //
  port[2].set(PULL_UD_SEL, 0xF0);  //  Pull-up selected for port2 bit 7~4
}

void loop() {
  int input2 = port[2];   //  Read port2 input
  port[2] = input2 >> 4;  //  Output to port2. Higher 4 bits are input. Those are shifted to lower 4 bits

  GPIO_base::print_bin(input2);  //  Show the data on serial terminal
  Serial.println("");
  delay(100);
}
