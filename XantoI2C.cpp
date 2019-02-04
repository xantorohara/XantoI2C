#include "XantoI2C.h"

XantoI2C::XantoI2C(uint8_t clock_pin, uint8_t data_pin, uint16_t delay_time_us):
  clock_pin(clock_pin), data_pin(data_pin), delay_time_us(delay_time_us) {
  sdaHi();
  sclHi();
}

void XantoI2C::i2cDelay() {
  delayMicroseconds(delay_time_us);
}

void XantoI2C::sclHi() {
  pinMode(clock_pin, INPUT_PULLUP);
  i2cDelay();
}

void XantoI2C::sdaHi() {
  pinMode(data_pin, INPUT_PULLUP);
  i2cDelay();
}

void XantoI2C::sclLo() {
  digitalWrite(clock_pin, LOW);
  pinMode(clock_pin, OUTPUT);
  i2cDelay();
}

void XantoI2C::sdaLo() {
  digitalWrite(data_pin, LOW);
  pinMode(data_pin, OUTPUT);
  i2cDelay();
}

void XantoI2C::start() {
  sdaHi();
  sclHi();
  i2cDelay();
  sdaLo();
  i2cDelay();
  sclLo();
  i2cDelay();
}

void XantoI2C::stop() {
  sdaLo();
  i2cDelay();
  sclHi();
  i2cDelay();
  sdaHi();
  i2cDelay();
}

void XantoI2C::clockPulse() {
  sclHi();
  i2cDelay();
  sclLo();
}

void XantoI2C::writeByte(uint8_t data_byte) {
  for (uint8_t i = 0; i < 8; i++) {
    if (bitRead(data_byte, 7 - i)) {
      sdaHi();
    } else {
      sdaLo();
    }
    clockPulse();
  }
}

uint8_t XantoI2C::readBit() {
  uint8_t out_bit;

  sclHi();
  out_bit = digitalRead(data_pin);
  sclLo();

  return out_bit;
}

uint8_t XantoI2C::readByte() {
  uint8_t out_byte = 0;

  sdaHi();
  for (uint8_t i = 0; i < 8; i++) {
    bitWrite(out_byte, 7 - i, readBit());
  }

  return out_byte;
}

/**
 * Return 0 if ACK was received, else 1
 */
uint8_t XantoI2C::readAck() {
  sdaHi();
  return readBit() == 0 ? 0 : 1;
}

/**
 * Return 0 if NACK was received, else 1
 */
uint8_t XantoI2C::readNack() {
  sdaHi();
  return readBit() == 1 ? 0 : 1;
}

/**
 * Return 0 if all steps were executed, else 1
 */
uint8_t XantoI2C::doStartWriteAckStop(uint8_t data_byte) {
  start();
  writeByte(data_byte);

  if (readAck()) {
    return 1;
  }
  stop();
  return 0;
}

/**
 * Return 0 if all steps were executed, else 1
 */
uint8_t XantoI2C::doStartWriteAckStop(uint8_t data_bytes[], uint8_t data_length) {
  start();
  for (uint8_t i = 0; i < data_length; i++) {
    writeByte(data_bytes[i]);
    if (readAck()) {
      return 1;
    }
  }
  stop();
  return 0;
}

