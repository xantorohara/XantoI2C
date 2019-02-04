/**
 * This is an example of usage XantoI2C for communicating with TM1637 chip.
 * This code demonstrates displaying numbers on a 4-digit LED display.
 * 
 * Author: Xantorohara <xantorohara@gmail.com>
 * Url: https://github.com/xantorohara/XantoI2C/tree/master/examples/XantoI2C_TM1637_Counter
 */

#include "XantoI2C.h"

const uint8_t PIN_DISPLAY_CLK = 5;
const uint8_t PIN_DISPLAY_DIO = 6;

const uint8_t TM1637_SEGMENTS[] = {
  0xFC, //0
  0x60, //1
  0xDA, //2
  0xF2, //3
  0x66, //4
  0xB6, //5
  0xBE, //6
  0xE0, //7
  0xFE, //8
  0xF6, //9
};

XantoI2C i2c(PIN_DISPLAY_CLK, PIN_DISPLAY_DIO);

uint32_t counter = 0;
uint8_t digits[] = {0, 0, 0, 0};

void convertNumberToDigits(uint16_t number, uint8_t digits[]) {
  digits[3] = number / 1000 % 10;
  digits[2] = number / 100 % 10;
  digits[1] = number / 10 % 10;
  digits[0] = number % 10;
}

void convertDigitsToSegments(uint8_t digits[]) {
  for (uint8_t i = 0; i < 4; i++) {
    digits[i] = TM1637_SEGMENTS[digits[i]];
  }
}

void displayNumber(uint16_t number, uint8_t display_point = 0) {
  convertNumberToDigits(number, digits);
  convertDigitsToSegments(digits);

  if (display_point) {
    digits[2] |= 0x01;
  }

  uint8_t data[] = {0x03, digits[3], digits[2], digits[1], digits[0]};

  // Command1 from the datasheet
  i2c.doStartWriteAckStop(0x02);

  // Command2 from the datasheet
  i2c.doStartWriteAckStop(data, 5);

  // Command3 from the datasheet
  i2c.doStartWriteAckStop(0xF1);
}

/**
 * Test all digits.
 * Output demo sequence: 1111, 2222, 3333, ..., 9999
 */
void testDisplay() {
  for (uint8_t i = 0; i < 10; i++) {
    displayNumber(i * 1111);
    delay(1000);
  }
}

void setup() {
  testDisplay();
}

void loop() {
  counter++;
  // display incremented counter, with a ":" symbol no each odd step
  displayNumber(counter, counter % 2);
  delay(500);
}

