# XantoI2C 
## :construction: Work in progress :construction:

Software I2C library implementation for Arduino

## Quick start
1. Download this library [XantoI2C-master.zip](https://github.com/xantorohara/XantoI2C/archive/master.zip)
2. Install the library (for example, via Arduino IDE: Sketch -> Include Library -> Add .ZIP Library...)
3. Write code that you need to interact with your chip using XantoI2C

This dummy example demonstrates basic usage of the library:

```cpp
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
```

Serial Data Line (SDA) and Serial Clock Line (SCL)

![Timings diagram ](https://github.com/xantorohara/XantoI2C/raw/master/extras/XantoI2C-timings.png?raw=true)

## XantoI2C Class API

### Class methods

```cpp

/**
 * Create a new I2C bus with the given speed
 *
 * clock_pin - any digital pin for SCL line
 * data_pin - any digital pin for SDA line
 * speed - bus speed in kilohertz (see "Bus Speed" section for speed explanations)
 */
XantoI2C(uint8_t clock_pin, uint8_t data_pin, uint16_t speed_kHz = 100);

/**
 * Create a new I2C bus with the given delay times
 *
 * clock_pin - any digital pin for SCL line
 * data_pin - any digital pin for SDA line
 * pulse_time_us - time when clock pulse is HIGH
 * delay_time_us - time of signal propagation, actually it is a half of a pulse time
 */
XantoI2C(uint8_t clock_pin, uint8_t data_pin, uint16_t pulse_time_us, uint16_t delay_time_us);

/**
 * Start Condition (or Repeated Start Condition)
 * First pulls the SDA line low, and next pulls the SCL line low.
 */
void start();

/**
 * I2C stop condition
 * The Bus Master first releases the SCL and then the SDA line.
 */
void stop();

/**
 * For each clock pulse one bit of data is transferred.
 * The SDA signal can only change when the SCL signal is low �
 * when the clock is high the data should be stable.
 */
void writeByte(uint8_t data_byte);

/**
 * Read one byte of data from a slave
 */
uint8_t readByte();

/**
 * Read Acknowledge from a slave
 * Return 0 if ACK was received, else 1
 */
uint8_t readAck();

/**
* Read No Acknowledge from a slave
* Return 0 if NACK was received, else 1
*/
uint8_t readNack();

```

Also it includes couple of common scenarios:
```cpp

/**
 * Execute common scenario: start, write, ack and stop
 */
uint8_t doStartWriteAckStop(uint8_t data_byte);

/**
 * Execute common scenario: start, multiple writes with acks and stop
 */
uint8_t doStartWriteAckStop(uint8_t data_bytes[], uint8_t data_length);

```

### Create instance

```cpp
XantoI2C i2c(PIN_SCL, PIN_SDA, 400);

```
### Usage1

```cpp

  i2c.doStartWriteAckStop(0x02);
  
```

### Usage2
```cpp

  i2c.start();

  i2c.writeByte(KT0803_CMD_WRITE);

  if (i2c.readAck()) {
    return 0;
  }

  i2c.writeByte(register_address);

  if (i2c.readAck()) {
    return 0;
  }

  i2c.start();
  i2c.writeByte(KT0803_CMD_READ);
  if (i2c.readAck()) {
    return 0;
  }
  uint8_t register_value = i2c.readByte();

  if (i2c.readNack()) {
    return 0;
  }
  i2c.stop();

```

## Examples
* [XantoI2C_TM1637_Counter](https://github.com/xantorohara/XantoI2C/tree/master/examples/XantoI2C_TM1637_Counter) - example 
how to drive a 4-digit LED display module using XantoI2C
* [XantoI2C_KT0803L_Registers](https://github.com/xantorohara/XantoI2C/tree/master/examples/XantoI2C_KT0803L_Registers) - 
example how to read all registers from the KT0803L chip (FM transmitter module)

## Related projects
* [XantoTM1637](https://github.com/xantorohara/XantoTM1637) - a library for a "Digital Tube" (4-digit display module).
XantoTM1637 uses XantoI2C library for I2C communications with TM1637 chip.
  
