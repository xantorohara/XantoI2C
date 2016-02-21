/**
 * XantoI2C
 * Software I2C library implementation for Arduino
 *
 * Version: 1.0.1
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
 * XantoI2C uses delays with microsecond precision, so the bus speed will
 * not be exactly the same as specified in the constructor, it will be slowly.
 *
 * Here are some samples of speed ranges:
 *
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
 *
 * I.e. if you specify speed as 120kHz or 110kHz or 101kHz it will be reduced to 100kHz.
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
     * Initialize SCL and SDA lines
     */
    void init();

    /**
     * Clock pulse
     */
    void clockPulse();

    /**
     * Read a bit of data from a slave
     */
    uint8_t readBit();

  public:
    uint16_t pulse_time_us, delay_time_us;

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

    /**
     * Pull both lines down, switch I2C communication off
     */
    void off();

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
