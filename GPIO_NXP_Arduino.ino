/** PCAL6524 GPIO operation sample
 *  
 *  This sample code is showing PCAL6524 GPIO operation.
 *
 *  *** IMPORTANT 0 ***
 *  *** TO RUN THIS SKETCH ON ARDUINO UNO R3P AND PCAL6xxx-ARD BOARDS, PIN10 MUST BE SHORTED TO PIN2 TO HANDLE INTERRUPT CORRECTLY
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6524:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/ic-bus-controller-and-bridge-ics/ultra-low-voltage-translating-24-bit-fm-plus-ic-bus-smbus-i-o-expander:PCAL6524?_gl=1*18ctsjm*_ga*NTA5NDE1NDA0LjE2NzgzNDYyNzA.*_ga_WM5LE0KMSH*MTY4MTM1NzI0NC40OC4wLjE2ODEzNTcyNDQuMC4wLjA.
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
    0xF0,  // Configure port2 as OUTPUT
  };

  gpio.config(io_config_and_pull_up);
  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);

  gpio.write_r8(PCAL6524::Interrupt_mask_register_port_2, (uint8_t)(~0xF0));
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
      print_bin(status[i]);

    input2 = gpio.input(2);

    Serial.print(",  input 2: ");
    print_bin(input2);
    Serial.println("");
  } else {
    input2 = gpio.input(2);
  }

  gpio.output(2, (input2 >> 4) & pat[count++ % sizeof(pat)]);
  delay(62);
}

void print_bin( uint8_t v )
{
  Serial.print(" 0b");
  for (int i = 7; 0 <= i; i-- )
    Serial.print(((v >> i) & 0x1) ? "1" : "0");
}
