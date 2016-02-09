#include "XantoI2C.h"

const uint8_t PIN_SCL = 8;
const uint8_t PIN_SDA = 9;

const uint8_t KT0803_CMD_WRITE = 0x7C;
const uint8_t KT0803_CMD_READ = 0x7D;

XantoI2C i2c(PIN_SCL, PIN_SDA, 400);

void setup() {
  Serial.begin(9600);

  Serial.print("Times: ");
  Serial.print(i2c.pulse_time_us);
  Serial.print(" ");
  Serial.print(i2c.delay_time_us);
  Serial.println("");
}


void register_dump(uint8_t register_address) {
  Serial.print(register_address, HEX);
  Serial.print("\t");
  uint8_t b = register_read(register_address);
  Serial.print(b, BIN);
  Serial.print("\t");
  Serial.println(b, HEX);
}

#define XANTO_DEBUG

uint8_t register_read(uint8_t register_address) {

  i2c.start();
#ifdef XANTO_DEBUG
  Serial.print("start ");
#endif

  i2c.writeByte(KT0803_CMD_WRITE);
#ifdef XANTO_DEBUG
  Serial.print("write_dev_addr ");
#endif

  if (i2c.ack()) {
#ifdef XANTO_DEBUG
    Serial.println("ack failed ");
#endif
    return 0;
  } else {
#ifdef XANTO_DEBUG
    Serial.print("ack ");
#endif

  }
  i2c.writeByte(register_address);
#ifdef XANTO_DEBUG
  Serial.print("write_reg_addr ");
  Serial.print(register_address, BIN);
#endif

  if (i2c.ack()) {
#ifdef XANTO_DEBUG
    Serial.println("ack2 failed");
#endif
    return 0;
  } else {
#ifdef XANTO_DEBUG
    Serial.print("ack2 ");
#endif

  }

  i2c.start();
#ifdef XANTO_DEBUG
  Serial.println("start2 ");
#endif

  i2c.writeByte(KT0803_CMD_READ);
#ifdef XANTO_DEBUG
  Serial.println("write_dev_addr2 ");
#endif

  if (i2c.ack()) {
#ifdef XANTO_DEBUG
    Serial.println("ack3 failed");
#endif
    return 0;

  } else {
#ifdef XANTO_DEBUG
    Serial.print("ack3 ");
#endif

  }

  uint8_t register_value = i2c.readByte();
#ifdef XANTO_DEBUG
  Serial.print("read ");
#endif

  if (i2c.nack()) {
#ifdef XANTO_DEBUG
    Serial.println("nack failed");
#endif
    return 0;

  } else {
#ifdef XANTO_DEBUG
    Serial.print("nack ");
#endif

  }

  i2c.stop();
#ifdef XANTO_DEBUG
  Serial.println("stop");
#endif

  return register_value;
}

void registers_dump() {
  Serial.println("Registers:");
  register_dump(0x00);

    register_dump(0x01);
    register_dump(0x02);
  /*  register_dump(0x04);
    register_dump(0x0B);
    register_dump(0x0C);
    register_dump(0x0E);
    register_dump(0x0F);
    register_dump(0x10);
    register_dump(0x12);
    register_dump(0x13);
    register_dump(0x14);
    register_dump(0x15);
    register_dump(0x16);
    register_dump(0x17);
    register_dump(0x1E);
    register_dump(0x26);
    register_dump(0x27);
    register_dump(0xFF);
  */
}

void loop() {
  registers_dump();
  delay(25000);
}
