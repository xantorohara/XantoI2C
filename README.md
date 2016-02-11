# XantoI2C 
Software I2C library implementation for Arduino


## Quick start
1. Download this library: [XantoI2C-master.zip](https://github.com/xantorohara/XantoI2C/archive/master.zip)
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
* It can be configured to work with unusual latency or propagation time,
for example when you have too long wires, capacitors, amplifiers or repeaters on the line,
or if you just use a barbed wires instead of copper wires


## Timings diagram
This pictures demonstrates some I2C cases. Also it shows signal timings (pulse time, delay time). 
 
![Timings diagram](https://github.com/xantorohara/XantoI2C/raw/master/extras/XantoI2C-timings.png?raw=true)


## Bus Speed
XantoI2C uses delays with microsecond precision, so the bus speed will
not be exactly the same as specified in the constructor, it will be a bit slowly.

Here are some samples of speed ranges:
* 1kHz - pulse=500us,delay=250us
* 2kHz - pulse=250us,delay=125us
* 5kHz - pulse=100us,delay=50us
* 10kHz - pulse=50us,delay=25us
* 25-26kHz - pulse=20us,delay=10us
* 50-55kHz - pulse=10us,delay=5us
* 56-62kHz - pulse=9us,delay=5us
* 63-71kHz - pulse=8us,delay=4us
* 72-83kHz - pulse=7us,delay=4us
* 84-99kHz - pulse=6us,delay=3us
* 100-124kHz - pulse=5us,delay=3us
* 125-166kHz - pulse=4us,delay=2us
* 167-249kHz - pulse=3us,delay=2us
* 250-499kHz - pulse=2us,delay=1us
* >500kHz - pulse=1us, delay=1us

I.e. if you specify speed as 120kHz or 110kHz or 101kHz it will be reduced to 100kHz.

Actually, many chips don't require strict speed for I2C communucation.
They declare maximal or nominal speed, but can operate on much lower speeds.

## Class API
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
- [ ] A lot of different I2C modules and displays you can find and order in 
[AliExpress](http://s.click.aliexpress.com/e/RZzf23vz3)

## Examples
* [XantoI2C_TM1637_Counter](https://github.com/xantorohara/XantoI2C/tree/master/examples/XantoI2C_TM1637_Counter) - example 
how to drive a 4-digit LED display module using XantoI2C
* [XantoI2C_KT0803L_Registers](https://github.com/xantorohara/XantoI2C/tree/master/examples/XantoI2C_KT0803L_Registers) - 
example how to read all registers from the KT0803L chip (FM transmitter module)


## Related projects
* [XantoTM1637](https://github.com/xantorohara/XantoTM1637) - a library for a "Digital Tube" (4-digit display module).
XantoTM1637 uses XantoI2C library for I2C communications with TM1637 chip.

  
## References
* [I2C Bus Specification](http://i2c.info/i2c-bus-specification)
* [I2C-Bus](http://www.i2c-bus.org)
* [I2C Bus Events](http://www.esacademy.com/en/library/technical-articles-and-documents/miscellaneous/i2c-bus/i2c-bus-events)
* :blue_book: [TM1637 datasheeet (English version)](http://xantorohara.github.io/datasheets/TM1637_V2.4_EN.pdf)
* :blue_book: [KT0803L datasheeet](http://xantorohara.github.io/datasheets/KT0803L.pdf)


## Author
Xantorohara <xantorohara@gmail.com>
