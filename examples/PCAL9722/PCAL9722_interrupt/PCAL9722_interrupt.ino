/** PCAL9722 GPIO operation sample
 *  
 *  This sample code is showing PCAL9722 GPIO operation.
 *
 *  *** IMPORTANT ***
 *  *** TO RUN THIS SKETCH ON ARDUINO UNO R3P AND PCAL9722-ARD BOARD, PIN7 MUST BE SHORTED TO PIN2 TO HANDLE INTERRUPT CORRECTLY
 *
 *  @author  Tedd OKANO
 *
 *  Released under the MIT license License
 *
 *  About PCAL9722:
 *    https://www.nxp.com/products/interfaces/ic-spi-i3c-interface-devices/general-purpose-i-o-gpio/22-bit-spi-i-o-expander-with-agile-i-o-features:PCAL9722
 */

#include <PCAL9722.h>

PCAL9722 gpio;

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

  SPI.begin();

  Serial.println("\n***** Hello, PCAL9722! *****");

  Serial.println("\n    *** If it seems the demo is not working, check the INT pins ***");
  Serial.println("    ***   D2<--->D7 pins should to be connected                ***");
  Serial.println("");
  Serial.println("8 LEDs on the ARD board blinks back and forth. Press button to generate interrupt.");
  Serial.println("The interrupt event will be shown on those LED and serial console");
  Serial.println("");

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0x00,  // Configure port1 as OUTPUT
    0x3F,  // Configure port2 bit 7~4 as INTPUT
  };

  gpio.config(io_config_and_pull_up);                   //  Port0, 1 and port2 bit 3~0 are configured as output
  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);   //  Pull-up/down enabled for port2 bit 7~4
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);  //  Pull-up selected for port2 bit 7~4

  gpio.write_port(INT_MASK, (uint8_t)(~0x3F), 2);  //  Interrupt mask cleared pn port2 bit 3~0

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), pin_int_callback, FALLING);
}

void loop() {
  static uint8_t num_pat[] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0xFF };
  static uint8_t led_pat[] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD };
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
    gpio.output(1, num_pat[find_bit(input2, 0)]);

    Serial.print(",  input 2: ");
    GPIO_base::print_bin(input2);
    Serial.println("");
  } else {
    input2 = gpio.input(2);
  }

  gpio.output(0, (input2 | 0xC0) & led_pat[count++ % sizeof(led_pat)]);
  delay(80);
}


int find_bit(int v, bool pol) {
  v = pol ? v : ~v;

  for (int i = 0; i < 8; i++)
    if ((v >> i) & 0x1)
      return i;

  return 6;
}