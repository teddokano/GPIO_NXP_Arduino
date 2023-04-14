/** PCAL6416A GPIO operation sample
 *  
 *  This sample code is showing PCAL6416A GPIO operation.
 *
 *  *** IMPORTANT ***
 *  *** TO RUN THIS SKETCH ON ARDUINO UNO R3P AND PCAL6xxx-ARD BOARDS, PIN10 MUST BE SHORTED TO PIN2 TO HANDLE INTERRUPT CORRECTLY
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6416A:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/low-voltage-translating-16-bit-ic-bus-smbus-i-o-expander:PCAL6416A
 */

#include <GPIO_NXP.h>

PCAL6416A gpio;

const uint8_t interruptPin = 2;
bool int_flag = false;

void pin_int_callback() {
  int_flag = true;
}

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address

  Serial.begin(9600);
  Serial.println("\n***** Hello, PCAL6416A! *****");

  Serial.println("\n    *** If it seems the demo is not working, check the INT pins ***");
  Serial.println(  "    ***   D2<--->D10 pins should to be connected                ***");
  Serial.println("");
  Serial.println("8 LEDs on the ARD board blinks back and forth. Press button to generate interrupt.");
  Serial.println("The interrupt event will be shown on those LED and serial console");
  Serial.println("");

  Wire.begin();

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0xFF,  // Configure port1 as INTPUT
  };

  gpio.config(io_config_and_pull_up);                   //  Port0 bit7~4 are INPUT, bit3~0 are are OUTPUT
  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);   //  Pull-up/down in port0 bit7~4 are enabled
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);  //  Pull-up/down in port0 bit7~4 are as pull-up

  gpio.write_port(INT_MASK, (uint8_t)(~0xFF), 1);       //  Interrupt mask cleared on port1

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pin_int_callback, FALLING);
}

void loop() {
  static uint8_t pat[] = {  //  LED blinking pattern
    0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 
  };
  static int count = 0;
  uint8_t input1;

  if (int_flag) {
    int_flag = false;

    uint8_t status[gpio.n_ports];

    gpio.read_port(INT_STATUS, status);

    Serial.print("[INT] status 0~1:");
    for (int i = 0; i < gpio.n_ports; i++)
      GPIO_base::print_bin(status[i]);

    input1 = gpio.input(1);

    Serial.print(",  input 1: ");
    GPIO_base::print_bin(input1);
    Serial.println("");
  } else {
    input1 = gpio.input(1);
  }

#if 0
  GPIO_base::print_bin(input1);
  GPIO_base::print_bin(pat[count % sizeof(pat)]);
  Serial.println("");
#endif

  gpio.output(0, input1 & pat[count++ % sizeof(pat)]);
  delay(62);
}
