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

  port[0].config(0x00);  //  Configure port0 as OUTPUT
}

void loop() {
  static int count = 0;
  port[0] = count++;  //  Output to port0
  delay(100);
}
