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

uint32_t i = 0;
uint8_t digits[] = {0, 0, 0, 0};


void displayNumber(uint16_t number, uint8_t display_point = 0) {
  convertNumberToDigits(number, digits);
  convertDigitsToSegments(digits);

  i2c.doStartWriteAckStop(0x02);

  if (display_point) {
    digits[2] |= 0x01;
  }

  uint8_t data[] = {0x03, digits[3], digits[2], digits[1], digits[0]};
  i2c.doStartWriteAckStop(data, 5);

  i2c.doStartWriteAckStop(0xF1);
}

void setup() {
  displayNumber(0);
  delay(1000);
  displayNumber(1);
  delay(1000);
  displayNumber(12);
  delay(1000);
  displayNumber(123);
  delay(1000);
  displayNumber(1234);
  delay(1000);
  displayNumber(2345);
  delay(1000);
  displayNumber(3456);
  delay(1000);
  displayNumber(4567);
  delay(1000);
  displayNumber(5678);
  delay(1000);
  displayNumber(6789);
  delay(1000);
  displayNumber(7890);
  delay(1000);
  displayNumber(8900);
  delay(1000);
  displayNumber(9000);
  delay(1000);
  displayNumber(0);
  delay(1000);

}

void loop() {
  i++;
  displayNumber(i, i % 2);
  delay(250);
}

