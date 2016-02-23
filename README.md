# XantoI2C 
Software I2C library implementation for Arduino


## Quick start
1. Download XantoI2C library: [XantoI2C-master.zip](https://github.com/xantorohara/XantoI2C/archive/master.zip)
2. Install the library (for example, via Arduino IDE: Sketch -> Include Library -> Add .ZIP Library...)
3. Write code that you need to interact with your chip using XantoI2C

This dummy sample demonstrates basic usage of the library:

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


## About XantoI2C
Simple software I2C-master implementation.
Like any other I2C it uses two wires to communicate with slave devices:
Serial Data Line (SDA) and Serial Clock Line (SCL). 
But it is not limited to the use of Arduino's special SDA/SCL pins, you can use **any** pins.

Features:
* Pure Arduino/C++ code
* No ASM, no magic, just plain clear code corresponding to the timing diagram
* It can communicate with I2C chips using any Arduino's pins, not only those that are marked as SDA and SCL,
so you can connect many more I2C devices to the Arduino at the same time.


## Timings diagram
This pictures demonstrates some I2C cases. Also it shows signal timings (pulse time, delay time). 
 
![Timings diagram](https://github.com/xantorohara/XantoI2C/raw/master/extras/XantoI2C-timings.png?raw=true)


## Bus Speed
XantoI2C uses delays with a microsecond precision to generate clock and data signals.

List of supported speeds:

* Delay=1us, speed=250kHz
* Delay=2us, speed=125kHz
* Delay=3us, speed=83kHz
* Delay=4us, speed=62kHz
* Delay=5us, speed=50kHz
* Delay=6us, speed=41kHz
* Delay=7us, speed=35kHz
* Delay=8us, speed=31kHz
* Delay=9us, speed=27kHz
* Delay=10us, speed=25kHz

... delays more than 10 microsecond are also possible.

Actually, many chips don't require strict speed for I2C communucation.
They declare maximal or nominal speed, but can operate on much lower speeds.

## Class API
```cpp

/**
 * Create a new I2C bus with the given speed
 *
 * clock_pin - any digital pin for SCL line
 * data_pin - any digital pin for SDA line
 * delay_time_us - time of signal propagation in microseconds
 * actually it is a quarter of the clock time, see Bus Speed section
 */
XantoI2C(uint8_t clock_pin, uint8_t data_pin, uint16_t delay_time_us = 2);

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
 * The SDA signal can only change when the SCL signal is low
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

Also XantoI2C includes some helper methods for frequently used scenarios:
```cpp

/**
 * Execute scenario: start, write, ack and stop
 */
uint8_t doStartWriteAckStop(uint8_t data_byte);

/**
 * Execute scenario: start, multiple writes with acks and stop
 */
uint8_t doStartWriteAckStop(uint8_t data_bytes[], uint8_t data_length);

```


### Sample: create instance of the I2C bus
```cpp
XantoI2C i2c(PIN_SCL, PIN_SDA);

```
### Sample: using a helper method
```cpp

i2c.doStartWriteAckStop(0x77);
  
```


### Sample: reading register value from a KT0803 chip
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
## I2C devices
A lot of different I2C modules and displays for Arduino you can find and order in 
[AliExpress](http://s.click.aliexpress.com/e/RZzf23vz3)

## Examples
* [XantoI2C_TM1637_Counter](https://github.com/xantorohara/XantoI2C/tree/master/examples/XantoI2C_TM1637_Counter) -
example how to drive a 4-digit LED display module using XantoI2C

* [XantoI2C_KT0803L_Registers](https://github.com/xantorohara/XantoI2C/tree/master/examples/XantoI2C_KT0803L_Registers) -
example how to read all registers from the KT0803L chip (FM transmitter module)


## Related projects
* [XantoTM1637](https://github.com/xantorohara/XantoTM1637) - 
Arduino library for TM1637-based 4-digit display module.
XantoTM1637 uses XantoI2C library for I2C communications with a TM1637 chip.

* [XantoKT0803](https://github.com/xantorohara/XantoKT0803) - 
Arduino library for KT0803-based FM radio transmitter.
XantoKT0803 uses XantoI2C library for I2C communications with a KT0803 chip.

  
## References
* [I2C Bus Specification](http://i2c.info/i2c-bus-specification)
* [I2C-Bus](http://www.i2c-bus.org)
* [I2C Bus Events](http://www.esacademy.com/en/library/technical-articles-and-documents/miscellaneous/i2c-bus/i2c-bus-events)
* :blue_book: [TM1637 datasheet (English version)](http://xantorohara.github.io/datasheets/TM1637_V2.4_EN.pdf)
* :blue_book: [KT0803L datasheet](http://xantorohara.github.io/datasheets/KT0803L.pdf)


## Author
Xantorohara <xantorohara@gmail.com>
