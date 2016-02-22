#include "XantoI2C.h"
#include <Math.h>

XantoI2C::XantoI2C(uint8_t clock_pin, uint8_t data_pin, uint16_t pulse_time_us, uint16_t delay_time_us):
  clock_pin(clock_pin), data_pin(data_pin), pulse_time_us(pulse_time_us), delay_time_us(delay_time_us) {
  init();
}

XantoI2C::XantoI2C(uint8_t clock_pin, uint8_t data_pin, uint16_t speed_kHz):
  clock_pin(clock_pin), data_pin(data_pin) {
  pulse_time_us = ceil(1000.0 / speed_kHz / 2);
  delay_time_us = ceil(1000.0 / speed_kHz / 4);
  init();
}

/**
 * Before: SCL is any, SDA is any
 * After: SCL is high, SDA is high
 */
void XantoI2C::init() {
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);
  digitalWrite(clock_pin, HIGH);
  digitalWrite(data_pin, HIGH);
}

/**
 * Before: SCL is any, SDA is any
 * After: SCL is low, SDA is low
 */
void  XantoI2C::off() {
  digitalWrite(clock_pin, LOW);
  digitalWrite(data_pin, LOW);
}

/**
 * Before: SCL is any, SDA is any
 * After: SCL is low, SDA is low
 */
void XantoI2C::start() {
  digitalWrite(data_pin, HIGH);
  delayMicroseconds(delay_time_us);
  digitalWrite(clock_pin, HIGH);
  delayMicroseconds(pulse_time_us);
  digitalWrite(data_pin, LOW);
  delayMicroseconds(pulse_time_us);
  digitalWrite(clock_pin, LOW);
  delayMicroseconds(pulse_time_us);
}

/**
 * Before: SCL is low, SDA is any
 * After: SCL is high, SDA is high
 */
void XantoI2C::stop() {
  digitalWrite(data_pin, LOW);
  delayMicroseconds(pulse_time_us);
  digitalWrite(clock_pin, HIGH);
  delayMicroseconds(pulse_time_us);
  digitalWrite(data_pin, HIGH);
  delayMicroseconds(pulse_time_us);
}

/**
 * Before: SCL is low
 * After: SCL is low
 */
void XantoI2C::clockPulse() {
  digitalWrite(clock_pin, HIGH);
  delayMicroseconds(pulse_time_us);
  digitalWrite(clock_pin, LOW);
  delayMicroseconds(delay_time_us);
}

/**
 * Before: SCL is low, SDA is any
 * After: SCL is low, SDA is last bit
 */
void XantoI2C::writeByte(uint8_t data_byte) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(data_pin, bitRead(data_byte, 7 - i) ? HIGH : LOW);
    delayMicroseconds(delay_time_us);
    clockPulse();
  }
}

/**
 * Before: SDA is in INPUT_PULLUP mode
 */
uint8_t XantoI2C::readBit() {
  uint8_t out_bit;
  delayMicroseconds(delay_time_us);

  digitalWrite(clock_pin, HIGH);
  delayMicroseconds(delay_time_us);

  out_bit = digitalRead(data_pin);

  digitalWrite(clock_pin, LOW);
  delayMicroseconds(delay_time_us);

  return out_bit;
}

/**
 * Before: SCL is low, SDA is any
 * After: SCL is high, SDA is any
 */
uint8_t XantoI2C::readByte() {
  uint8_t out_byte = 0;
  pinMode(data_pin, INPUT_PULLUP);

  for (uint8_t i = 0; i < 8; i++) {
    bitWrite(out_byte, 7 - i, readBit());
  }

  pinMode(data_pin, OUTPUT);
  return out_byte;
}

/**
 * Before: SCL is low, SDA is any
 * After: SCL is low, SDA is any
 * Return 0 if ACK was received, else 1
 */
uint8_t XantoI2C::readAck() {
  pinMode(data_pin, INPUT_PULLUP);
  uint8_t tmp_bit = readBit();
  pinMode(data_pin, OUTPUT);
  return tmp_bit == 0 ? 0 : 1;
}

/**
 * Before: SCL is low, SDA is any
 * After: SCL is low, SDA is any
 * Return 0 if NACK was received, else 1
 */
uint8_t XantoI2C::readNack() {
  pinMode(data_pin, INPUT_PULLUP);
  uint8_t tmp_bit = readBit();
  pinMode(data_pin, OUTPUT);
  return tmp_bit == 1 ? 0 : 1;
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
