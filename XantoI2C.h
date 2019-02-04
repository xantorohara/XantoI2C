/**
 * XantoI2C
 * Software I2C library implementation for Arduino
 *
 * Version: 1.0.2
 * Author: Xantorohara <xantorohara@gmail.com>
 *
 * References:
 * -----------
 * - http://i2c.info/i2c-bus-specification
 * - http://www.i2c-bus.org/repeated-start-condition
 * - http://www.esacademy.com/en/library/technical-articles-and-documents/miscellaneous/i2c-bus/i2c-bus-events
 * - TM1637 datasheet http://xantorohara.github.io/datasheets/TM1637_V2.4_EN.pdf
 * - KT0803L datasheet http://xantorohara.github.io/datasheets/KT0803L.pdf
 *
 * Bus Speed:
 * ----------
 * XantoI2C uses delays with a microsecond precision to generate clock and data signals.
 *
 * List of supported speeds:
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
 * ... delays more than 10us are also possible.
 *
 * Actually, many chips don't require strict speed for I2C communucation.
 * They declare maximal or nominal speed, but can operate on much lower speeds.
 *
 * Advantages:
 * -----------
 * - Pure Arduino/C++ code
 * - No ASM, no magic, just plain clear code corresponding to the timing diagram
 * - It can communicate with I2C chips using any Arduino's pins,
 *   not only those that are marked as SDA and SCL
 * - It can be configured to work with unusual latency or propagation time,
 *   when you have too long wires, capacitors or repeaters on the line,
 *   or if you just use a barbwire instead of wires
 *
 */

#ifndef XANTO_I2C
#define XANTO_I2C

#include <Arduino.h>

class XantoI2C {
  private:
    uint8_t clock_pin, data_pin;

    /**
     * Clock pulse
     */
    void clockPulse();

    /**
     * Read a bit of data from a slave
     */
    uint8_t readBit();

    void sclHi();
    void sdaHi();
    void sclLo();
    void sdaLo();
    void i2cDelay();

  public:
    uint16_t delay_time_us;

    /**
     * Create a new I2C bus with the given speed
     *
     * clock_pin - any digital pin for SCL line
     * data_pin - any digital pin for SDA line
     * delay_time_us - time of signal propagation, (quarter of the clock time)
     * see Bus Speed section
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

    /**
     * Execute scenario: start, write, ack and stop
     */
    uint8_t doStartWriteAckStop(uint8_t data_byte);

    /**
     * Execute scenario: start, multiple writes with acks and stop
     */
    uint8_t doStartWriteAckStop(uint8_t data_bytes[], uint8_t data_length);

};
#endif XANTO_I2C
