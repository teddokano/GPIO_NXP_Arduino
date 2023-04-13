/** PCAL6524 GPIO operation sample
 *  
 *  This sample code is showing PCAL6524 GPIO operation.
 *
 *  *** IMPORTANT ***
 *  *** TO RUN THIS SKETCH ON ARDUINO UNO R3P AND PCAL6xxx-ARD BOARDS, PIN10 MUST BE SHORTED TO PIN2 TO HANDLE INTERRUPT CORRECTLY
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6524:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/ic-bus-controller-and-bridge-ics/ultra-low-voltage-translating-24-bit-fm-plus-ic-bus-smbus-i-o-expander:PCAL6524
 */

#include "GPIO_NXP.h"

PCAL6524 gpio;

const uint8_t interruptPin = 2;
bool int_flag = false;

void pin_int_callback() {
  int_flag = true;
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCAL6524! *****");

  Serial.println("\n    *** If it seems the demo is not working, check the INT pins ***");
  Serial.println("    ***   D2<--->D10 pins should to be connected       ***");
  Serial.println("");
  Serial.println("4 LEDs on the ARD board blinks back and forth. Press button to generate interrupt.");
  Serial.println("The interrupt event will be shown on those LED and serial console");
  Serial.println("");

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pin_int_callback, FALLING);

  Wire.begin();

  I2C_device::scan();

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0x00,  // Configure port1 as OUTPUT
    0xF0,  // Configure port2 bit 7~4 as INTPUT
  };

  gpio.config(io_config_and_pull_up);
  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);

  gpio.write_port(INT_MASK, (uint8_t)(~0xF0), 2);
}

void loop() {
  static uint8_t pat[] = {  //  LED blinking pattern
    0xFE, 0xFB, 0xFD, 0xF7, 0xFD, 0xFB, 
  };
  static int count = 0;
  uint8_t input2;

  if (int_flag) {
    int_flag = false;

    uint8_t status[gpio.n_ports];

    gpio.read_port(INT_STATUS, status);

    Serial.print("[INT] status 0~2:");
    for (int i = 0; i < gpio.n_ports; i++)
      GPIO_base::print_bin(status[i]);

    input2 = gpio.input(2);

    Serial.print(",  input 2: ");
    GPIO_base::print_bin(input2);
    Serial.println("");
  } else {
    input2 = gpio.input(2);
  }

  gpio.output(2, (input2 >> 4) & pat[count++ % sizeof(pat)]);
  delay(80);
}
