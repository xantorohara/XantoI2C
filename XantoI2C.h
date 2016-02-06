/**
 * Software I2C implementation for Arduino
 *
 * Version: 2016-01-31
 * Author: Alex Bell (xantorohara@gmail.com)
 * References:
 * ----------
 * - http://i2c.info/i2c-bus-specification
 * - http://www.i2c-bus.org/repeated-start-condition
 * - http://www.esacademy.com/en/library/technical-articles-and-documents/miscellaneous/i2c-bus/i2c-bus-events
 * - TM1637 datasheeet
 * - KT0803L datasheet
 *
 *
 * Bus Speed:
 * ----------
 * XantoI2C uses delays with a microsecond precision,
 * so the bus speed will be not exactly the same as specified in the constructor.
 *
 * Here are samples of speed ranges:
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
 * Actually, many chips don't require strict speed for I2C communucation.
 * They declare maximal or nominal speed, but can operate on much lower speeds.
 */

#ifndef XANTO_I2C
#define XANTO_I2C

#include <Arduino.h>


class XantoI2C {
  private:
    uint8_t clock_pin, data_pin;

    /**
     * Initialize SCL and SDA lines.
     * As a result, both lines are output HIGH.
     */
    void init();

    /**
     * Clock pulse: set the SCL line HIGH, wait pulse_time, set it LOW, wait delay_time
     */
    void clockPulse();
    
    uint8_t readBit();
    
  public:
    uint16_t pulse_time_us, delay_time_us;

    /**
     * Create new I2C bus with the given speed
     *
     * data_pin - any digital pin for SDA line
     * clock_pin - any digital pin for SCL line
     * speed - bus speed in kilohertz (see "Bus Speed" section for speed explanations)
     */
    XantoI2C(uint8_t clock_pin, uint8_t data_pin, uint16_t speed_kHz = 100);

    /**
     * Create new I2C bus with the given delay times
     *
     * data_pin - any digital pin for SDA line
     * clock_pin - any digital pin for SCL line
     * pulse_time_us - time when clock line is HIGH
     * delay_time_us - time of signal propagation, actually it is a half of a pulse time
     *
     * You can use this constructor to create and tune the bus for some specifics cases
     * (unusual latency or propagation time,  capacitors or repeaters on the line,
     * or if you just use a barbwire instead of wires)
     */
    XantoI2C(uint8_t clock_pin, uint8_t data_pin, uint16_t pulse_time_us, uint16_t delay_time_us);


    /**
     * Start Condition
     * or 
     * Repeated Start Condition
     * First pulls the SDA line low, and next pulls the SCL  line low.
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

    uint8_t readByte();

    uint8_t ack();
    
    uint8_t nack();


    /**
     * Pull both lines down, switch I2C communication off
     */
    void off();


    /**
     * Execute common scenario: start, write, ack, then stop
     */
    uint8_t doStartWriteAckStop(uint8_t data_byte);

    /**
     * Execute common scenario: start, multiple writes with acks, then stop
     */
    uint8_t doStartWriteAckStop(uint8_t data_bytes[], uint8_t data_length);

};
#endif XANTO_I2C
