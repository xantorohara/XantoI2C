# XantoI2C

Software I2C library implementation for Arduino

:construction: Work in progress :construction:

Serial Data Line (SDA) and Serial Clock Line (SCL)

![Timings diagram ](https://github.com/xantorohara/XantoI2C/raw/master/extras/XantoI2C-timings.png?raw=true)

## Class API

XantoI2C class methods

```c

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
 * The SDA signal can only change when the SCL signal is low –
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
```c

/**
 * Execute common scenario: start, write, ack and stop
 */
uint8_t doStartWriteAckStop(uint8_t data_byte);

/**
 * Execute common scenario: start, multiple writes with acks and stop
 */
uint8_t doStartWriteAckStop(uint8_t data_bytes[], uint8_t data_length);

```

## Examples
* [XantoI2C_TM1637_Counter](https://github.com/xantorohara/XantoI2C/tree/master/examples/XantoI2C_TM1637_Counter) - example 
how to drive a 4-digit LED display module using XantoI2C
* [XantoI2C_KT0803L_Registers](https://github.com/xantorohara/XantoI2C/tree/master/examples/XantoI2C_KT0803L_Registers) - 
example how to read all registers from the KT0803L chip (FM transmitter module)

## Related projects
* [XantoTM1637](https://github.com/xantorohara/XantoTM1637) - a library for a "Digital Tube" (4-digit display module).
XantoTM1637 uses XantoI2C library for I2C communications with TM1637 chip.
  
