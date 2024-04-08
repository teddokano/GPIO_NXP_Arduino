/** PCAL9722 GPIO operation sample
 *  
 *  This sample code is showing PCAL9722 GPIO operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL9722:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/22-bit-spi-i-o-expander-with-agile-i-o-features:PCAL9722
 */

#include <PCAL9722.h>
#include <PORT.h>

PCAL9722 gpio;
PORT port[] = { PORT(gpio, 0),
                PORT(gpio, 1),
                PORT(gpio, 2) };


void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address

  Serial.begin(9600);
  while (!Serial)
    ;

  SPI.begin();
  pinMode(SS, OUTPUT);  //  Required for UNO R4

  Serial.println("\n***** Hello, PCAL9722! *****");

  port[0].config(0x00);  //  Configure port0 as OUTPUT
  port[1].config(0x00);  //  Configure port1 as OUTPUT
  port[2].config(0x3F);  //  Configure port2 bit 7~4 as INTPUT

  port[0].set(PULL_UD_EN, 0x00);  //  Pull-up/down disabled
  port[1].set(PULL_UD_EN, 0x00);  //  Pull-up/down disabled
  port[2].set(PULL_UD_EN, 0x3F);  //  Pull-up/down enabled for port2 bit 7~4

  port[0].set(PULL_UD_SEL, 0x00);  //
  port[1].set(PULL_UD_SEL, 0x00);  //
  port[2].set(PULL_UD_SEL, 0x3F);  //  Pull-up selected for port2 bit 7~4
}

void loop() {
  int input2 = port[2];   //  Read port2 input
  port[0] = input2 | 0xC0;  //  Output to port2. Higher 2 bits are masked

  GPIO_base::print_bin(input2);  //  Show the data on serial terminal
  Serial.println("");
  delay(100);
}
