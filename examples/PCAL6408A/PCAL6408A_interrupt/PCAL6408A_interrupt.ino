/** PCAL6408A GPIO operation sample
 *  
 *  This sample code is showing PCAL6408A GPIO operation.
 *
 *  *** IMPORTANT ***
 *  *** TO RUN THIS SKETCH ON ARDUINO UNO R3P AND PCAL6xxx-ARD BOARDS, PIN10 MUST BE SHORTED TO PIN2 TO HANDLE INTERRUPT CORRECTLY
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL6408A:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/low-voltage-translating-8-bit-ic-bus-smbus-i-o-expander:PCAL6408A
 */

#include <PCAL6408A.h>

PCAL6408A gpio;

const uint8_t interruptPin = 2;
bool int_flag = false;

void pin_int_callback() {
  int_flag = true;
}

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address
  
  Serial.begin(9600);
  while (!Serial)
    ;

  Wire.begin();

  Serial.println("\n***** Hello, PCAL6408A! *****");

  Serial.println("\n    *** If it seems the demo is not working, check the INT pins ***");
  Serial.println(  "    ***   D2<--->D10 pins should to be connected                ***");
  Serial.println("");
  Serial.println("4 LEDs on the ARD board blinks back and forth. Press button to generate interrupt.");
  Serial.println("The interrupt event will be shown on those LED and serial console");
  Serial.println("");

  uint8_t io_config_and_pull_up = 0xF0;
  
  gpio.write_port(CONFIG, io_config_and_pull_up);       //  Port0 bit7~4 are INPUT, bit3~0 are are OUTPUT
  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);   //  Pull-up/down in port0 bit7~4 are enabled
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);  //  Pull-up/down in port0 bit7~4 are as pull-up
  
  gpio.write_port(INT_MASK, ~io_config_and_pull_up);    //  Clear mask bits for input

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pin_int_callback, FALLING);
}

void loop() {
  static uint8_t pat[] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xFB, 0xFD, }; //  LED blinking pattern
  static int count = 0;
  uint8_t input0;

  if (int_flag) {
    int_flag = false;

    uint8_t status  = gpio.read_port(INT_STATUS);

    Serial.print("[INT] status:");
    GPIO_base::print_bin(status);

    input0 = gpio.input(0);

    Serial.print(",  input: ");
    GPIO_base::print_bin(input0);
    Serial.println("");
  } else {
    input0 = gpio.input(0);
  }

  gpio.output(0, (input0 >> 4) & pat[count++ % sizeof(pat)]);

  delay(80);
}
