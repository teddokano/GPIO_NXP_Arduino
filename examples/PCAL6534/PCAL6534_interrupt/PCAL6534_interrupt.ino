/** PCAL6534 GPIO operation sample
 *  
 *  This sample code is showing PCAL6534 GPIO operation.
 *
 *  *** IMPORTANT ***
 *  *** TO RUN THIS SKETCH ON ARDUINO UNO R3P AND PCAL6xxx-ARD BOARDS, PIN10 MUST BE SHORTED TO PIN2 TO HANDLE INTERRUPT CORRECTLY
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6534:
 *    https://www.nxp.jp/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/ultra-low-voltage-level-translating-34-bit-ic-bus-smbus-i-o-expander:PCAL6534
 */

#include <GPIO_NXP.h>

PCAL6534 gpio;

const uint8_t interruptPin = 2;
bool int_flag = false;

void pin_int_callback() {
  int_flag = true;
}

void setup() {
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCAL6534! *****");

  Serial.println("\n    *** If it seems the demo is not working, check the INT pins ***");
  Serial.println("    ***   D2<--->D10 pins should to be connected       ***");
  Serial.println("");
  Serial.println("8 LEDs on the ARD board blinks back and forth. Press button to generate interrupt.");
  Serial.println("The interrupt event will be shown on those LED and serial console");
  Serial.println("");

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pin_int_callback, FALLING);

  Wire.begin();

  I2C_device::scan();

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

  gpio.write_port(INT_MASK, (uint8_t)(~0xE0), 3);
  gpio.write_port(INT_MASK, (uint8_t)(~0x03), 4);
}

void loop() {
  static uint8_t pat[] = {  //  LED blinking pattern
    0xFE, 0xFB, 0xFD, 0xEF, 0xF7, 0xBF, 0xDF,
    0x7F, 0xDF, 0xBF, 0xF7, 0xEF, 0xFD, 0xFB,
  };
  static int count = 0;
  uint8_t input3;
  uint8_t input4;

  if (int_flag) {
    int_flag = false;

    uint8_t status[gpio.n_ports];

    gpio.read_port(INT_STATUS, status);

    Serial.print("[INT] status 0~4:");
    for (int i = 0; i < gpio.n_ports; i++)
      GPIO_base::print_bin(status[i]);

    input3 = gpio.input(3);
    input4 = gpio.input(4);

    Serial.print(",  input 3 and 4: ");
    GPIO_base::print_bin(input3);
    GPIO_base::print_bin(input4);
    Serial.println("");
  } else {
    input3 = gpio.input(3);
    input4 = gpio.input(4);
  }

  gpio.output(2, ((input3 & 0xFC) | input4) & pat[count++ % sizeof(pat)]);
  delay(62);
}
