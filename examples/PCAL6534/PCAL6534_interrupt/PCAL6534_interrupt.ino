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
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/ultra-low-voltage-level-translating-34-bit-ic-bus-smbus-i-o-expander:PCAL6534
 */

#include <PCAL6534.h>

PCAL6534 gpio;

const uint8_t interruptPin = 2;
bool int_flag = false;

void pin_int_callback() {
  int_flag = true;
}

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address
  
  Serial.begin(9600);
  Serial.println("\n***** Hello, PCAL6534! *****");

  Serial.println("\n    *** If it seems the demo is not working, check the INT pins ***");
  Serial.println(  "    ***   D2<--->D10 pins should to be connected                ***");
  Serial.println("");
  Serial.println("8 LEDs on the ARD board blinks back and forth. Press button to generate interrupt.");
  Serial.println("The interrupt event will be shown on those LED and serial console");
  Serial.println("");

  Wire.begin();

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0x00,  // Configure port1 as OUTPUT
    0x00,  // Configure port2 as OUTPUT
    0xE0,  // Configure port3 bit 7~5 as INPUT
    0x03,  // Configure port4 bit 1 and 0 as INPUT
  };

  gpio.config(io_config_and_pull_up);                   //  Port0, 1, 2 and port3 bit 4~0 are configured as output
  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);   //  Pull-up/down enabled for port3 bit 7~5 and port4 bit 1 and 0
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);  //  Pull-up selected for port3 bit 7~5 and port4 bit 1 and 0

  gpio.write_port(INT_MASK, (uint8_t)(~0xE0), 3);       //  Interrupt mask cleared on port3 bit 7~5
  gpio.write_port(INT_MASK, (uint8_t)(~0x03), 4);       //  Interrupt mask cleared on port4 bit 1 and 0

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pin_int_callback, FALLING);
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
