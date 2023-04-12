/** PCAL6534 GPIO operation sample
 *  
 *  This sample code is showing PCAL6534 GPIO operation.
 *
 *  *** IMPORTANT 0 ***
 *  *** TO RUN THIS SKETCH ON ARDUINO UNO R3P AND PCAL6xxx-ARD BOARDS, PIN10 MUST BE SHORTED TO PIN2 TO HANDLE INTERRUPT CORRECTLY
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6534:
 *    https://www.nxp.jp/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/ultra-low-voltage-level-translating-34-bit-ic-bus-smbus-i-o-expander:PCAL6534
 */

#include "GPIO_NXP.h"

PCAL6534 gpio;

const uint8_t interruptPin = 2;
bool int_flag = false;

void pin_int_callback() {
  int_flag = true;
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCAL6534! *****");

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pin_int_callback, FALLING);

  Wire.begin();

  I2C_device::scan();

  //gpio.begin(GPIO_base::ARDUINO_SHIELD);

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0x00,  // Configure port1 as OUTPUT
    0x00,  // Configure port2 as OUTPUT
    0xE0,  // Configure port3 bit 7~5 as INPUT
    0x03,  // Configure port4 bit 1 and 0 as INPUT
  };
  gpio.config(io_config_and_pull_up);

  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);

  gpio.write_r8(PCAL6534::Interrupt_mask_register_port_3, (uint8_t)(~0xE0));
  gpio.write_r8(PCAL6534::Interrupt_mask_register_port_4, (uint8_t)(~0x03));

  Serial.println("    *** If it seems the demo is not working, check the INT pins ***");
  Serial.println("    ***   D2<--->D10 pins should to be connected       ***");

  Serial.println("size");
  Serial.println(gpio.n_bits);
  Serial.println(gpio.n_ports);
}

void loop() {
  if (int_flag) {
    int_flag = false;

    uint8_t status[gpio.n_ports];

    gpio.read_port(INT_STATUS, status);

    Serial.print("[INT] status:");

    for (int i = 0; i < gpio.n_ports; i++) {
      Serial.print(" ");
      Serial.print(status[i], HEX);
    }

    Serial.println("");
  }

  int input3 = gpio.input(3);
  int input4 = gpio.input(4);

  gpio.output(2, (input3 & 0xFC) | input4);

  Serial.print(" ");
  Serial.print(input3, HEX);
  Serial.print(" ");
  Serial.print(input4, HEX);
  Serial.println("");

  delay(100);
}
