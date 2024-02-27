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
}

void loop() {
  static int count = 0;
  port[2] = count++;  //  Output to port2
  delay(100);
}
