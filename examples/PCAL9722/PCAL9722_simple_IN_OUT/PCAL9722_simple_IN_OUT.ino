/** PCAL9722 GPIO operation sample
 *  
 *  This sample code is showing PCAL9722 GPIO operation.
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

void setup() {
  gpio.begin(GPIO_base::ARDUINO_SHIELD);  //  Force ADR pin (@D8) LOW and reset to give right target address

  Serial.begin(9600);
  while (!Serial)
    ;

  SPI.begin();

  Serial.println("\n***** Hello, PCAL9722! *****");

  uint8_t io_config_and_pull_up[] = {
    0x00,  // Configure port0 as OUTPUT
    0x00,  // Configure port1 as OUTPUT
    0x3F,  // Configure port2 bit 5~0 as INTPUT
  };

  gpio.config(io_config_and_pull_up);                   //  Port0 and 1 configured as output. port2 bit 5~0 are input
  gpio.write_port(PULL_UD_SEL, io_config_and_pull_up);  //  Pull-up selected for port2 bit 7~0
  gpio.write_port(PULL_UD_EN, io_config_and_pull_up);   //  Pull-up/down enabled for port2 bit 7~0
}

void loop() {
  static uint8_t num_pat[] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0xFF };
  static uint8_t led_pat[] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD };
  static int count = 0;

  int input2 = gpio.input(2);     //  Read port2 input
  gpio.output(0, (input2 | 0xC0) & led_pat[ count++ % sizeof( led_pat ) ]);
  gpio.output(1, num_pat[find_bit(input2, 0)]);

  GPIO_base::print_bin(input2);  //  Show the data on serial terminal
  Serial.println("");

  delay(50);
}

int find_bit(int v, bool pol) {
  v = pol ? v : ~v;

  for (int i = 0; i < 8; i++)
    if ((v >> i) & 0x1)
      return i;

  return 6;
}
