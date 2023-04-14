/** PCA9555 GPIO operation sample
 *  
 *  This sample code is showing PCA9555 GPIO operation.
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCA9555:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/16-bit-ic-bus-and-smbus-i-o-port-with-interrupt:PCA9555
 */

#include <GPIO_NXP.h>

PCA9555 gpio;

const uint8_t interruptPin = 2;
bool int_flag = false;

void pin_int_callback() {
  int_flag = true;
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCA9555! *****");

  Wire.begin();

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0xFF,  // Configure port1 as INTPUT
  };

  gpio.config(io_config_and_pull_up);  //  Port0 as OUTPUT, port1 as INPUT

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pin_int_callback, FALLING);
}
void loop() {
  if (int_flag) {
    int_flag = false;

    Serial.print("[INT] input: ");
    GPIO_base::print_bin(gpio.input(1));
    Serial.println("");
  }

  static int count = 0;
  gpio.output(0, count++);  //  Output to port0
  delay(100);
}
