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

  gpio.config(0, 0x00); //  Port0 is configured as output
}

void loop() {
  static int count = 0;
  gpio.output(0, count++);  //  Output to port0 
  delay(100);
}
