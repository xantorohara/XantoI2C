/**
 * This is an example of usage XantoI2C for communicating with KT0803L chip.
 * This code demonstrates dumping registers from a FM transmitter module.
 *
 * Author: Xantorohara <xantorohara@gmail.com>
 * Url: https://github.com/xantorohara/XantoI2C/tree/master/examples/XantoI2C_TM1637_Counter
 */

#include "XantoI2C.h"

#define XANTO_DEBUG_PRINT(message) (Serial.print(message))
#define XANTO_DEBUG_PRINT

const uint8_t PIN_SCL = 8;
const uint8_t PIN_SDA = 9;

const uint8_t KT0803_CMD_WRITE = 0x7C;
const uint8_t KT0803_CMD_READ = 0x7D;

XantoI2C i2c(PIN_SCL, PIN_SDA, 1);

void setup() {
  Serial.begin(9600);
}

uint8_t readRegister(uint8_t register_address) {
  i2c.start();
  XANTO_DEBUG_PRINT("start1 ");

  i2c.writeByte(KT0803_CMD_WRITE);
  XANTO_DEBUG_PRINT("write1 ");

  if (i2c.readAck()) {
    XANTO_DEBUG_PRINT("ack1 failed ");
    return 0;
  } else {
    XANTO_DEBUG_PRINT("ack1 ");
  }

  i2c.writeByte(register_address);
  XANTO_DEBUG_PRINT("write2 ");

  if (i2c.readAck()) {
    XANTO_DEBUG_PRINT("ack2 failed ");
    return 0;
  } else {
    XANTO_DEBUG_PRINT("ack2 ");
  }

  i2c.start();
  XANTO_DEBUG_PRINT("start2 ");

  i2c.writeByte(KT0803_CMD_READ);
  XANTO_DEBUG_PRINT("write3 ");

  if (i2c.readAck()) {
    XANTO_DEBUG_PRINT("ack3 failed ");
    return 0;
  } else {
    XANTO_DEBUG_PRINT("ack3 ");
  }

  uint8_t register_value = i2c.readByte();
  XANTO_DEBUG_PRINT("read ");

  if (i2c.readNack()) {
    XANTO_DEBUG_PRINT("nack failed ");
    return 0;
  } else {
    XANTO_DEBUG_PRINT("nack ");
  }

  i2c.stop();
  XANTO_DEBUG_PRINT("stop\n");

  return register_value;
}

void dumpRegister(uint8_t register_address) {
  Serial.print(register_address, HEX);
  Serial.print("\t");
  uint8_t value = readRegister(register_address);
  Serial.print(value, HEX);
  Serial.print("\t");
  Serial.println(value, BIN);
}

/**
 * Read register values from a KT0803L chip and print to Serial
 */
void dumpAllRegisters() {
  Serial.println("Registers: (Address, Hex value, Bin value)");

  dumpRegister(0x00);
  dumpRegister(0x01);
  dumpRegister(0x02);
  dumpRegister(0x04);
  dumpRegister(0x0B);
  dumpRegister(0x0C);
  dumpRegister(0x0E);
  dumpRegister(0x0F);
  dumpRegister(0x10);
  dumpRegister(0x12);
  dumpRegister(0x13);
  dumpRegister(0x14);
  dumpRegister(0x15);
  dumpRegister(0x16);
  dumpRegister(0x17);
  dumpRegister(0x1E);
  dumpRegister(0x26);
  dumpRegister(0x27);
  Serial.println("Done");
}

void loop() {
  dumpAllRegisters();
  delay(60000);
}
