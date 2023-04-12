/** PCAL6534 GPIO operation sample
 *  
 *  This sample code is showing PCAL6534 GPIO operation.
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

  gpio.begin(GPIO_base::ARDUINO_SHIELD);

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0x00,  // Configure port1 as OUTPUT
    0x00,  // Configure port2 as OUTPUT
    0xE0,  // Configure port3 bit 7~5 as INPUT
    0x03,  // Configure port4 bit 1 and 0 as INPUT
  };
  gpio.config(io_config_and_pull_up);
  gpio.all_port_w8(PCAL6534::Pull_up_pull_down_enable_register_port_0, io_config_and_pull_up);
  gpio.all_port_w8(PCAL6534::Pull_up_pull_down_selection_register_port_0, io_config_and_pull_up);

  gpio.write_r8(PCAL6534::Interrupt_mask_register_port_3, (uint8_t)(~0xE0));
  gpio.write_r8(PCAL6534::Interrupt_mask_register_port_4, (uint8_t)(~0x03));
}
void loop() {
#if 0
  static int count = 0;
  gpio.output(2, count++);
#else
  int int3 = gpio.read_r8(PCAL6534::Interrupt_status_register_port_3);
  int int4 = gpio.read_r8(PCAL6534::Interrupt_status_register_port_4);

  //  int input3 = gpio.input(3);
  //  int input4 = gpio.input(4);
  int input3 = gpio.read_r8(PCAL6534::Input_Port_3);
  int input4 = gpio.read_r8(PCAL6534::Input_Port_4);

  int intm3 = gpio.read_r8(PCAL6534::Interrupt_mask_register_port_3);
  int intm4 = gpio.read_r8(PCAL6534::Interrupt_mask_register_port_4);
  gpio.output(2, (input3 & 0xFC) | input4);

  Serial.print(" ");
  Serial.print(input3, HEX);
  Serial.print(" ");
  Serial.print(input4, HEX);
  Serial.print(" ");
  Serial.print(PCAL6534::Interrupt_mask_register_port_3, HEX);
  Serial.print(" ");
  Serial.print(intm3, HEX);
  Serial.print(" ");
  Serial.print(intm4, HEX);
  Serial.print(" ");
  Serial.print(PCAL6534::Interrupt_status_register_port_3, HEX);
  Serial.print(" ");
  Serial.print(int3, HEX);
  Serial.print(" ");
  Serial.print(int4, HEX);
  Serial.println("");

  if (int_flag) {
    int_flag = false;

    Serial.println("Interrupt:");
  }

#endif
  delay(100);
}
