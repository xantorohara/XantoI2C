#include <XantoI2C.h>

const uint8_t PIN_SCL = 5;
const uint8_t PIN_SDA = 6;
const uint8_t SOME_CHIP_COMMAND = 0x77;

XantoI2C i2c(PIN_SCL, PIN_SDA);

void sendSomeCommand() {
  i2c.start();
  i2c.writeByte(SOME_CHIP_COMMAND);

  if (i2c.readAck()) {
    return;
  }
  i2c.stop();
}

void setup() {
  sendSomeCommand();
}

void loop() {
}

