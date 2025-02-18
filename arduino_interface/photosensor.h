#ifndef __PHOTOSENSOR_H_
#define __PHOTOSENSOR_H_

#define INTER_BYTE_DELAY_MS 100

#include <Arduino.h>

// Pins for digital inputs
const int MOTION_PIN = NULL;
const int SDIO = D7;
const int SCLK = D8;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Run necessary functions to initiate communication over pins
 */
void photosensor_setup();

/**
 * Send a single byte to the sensor
 * Note: Function written with reference to https://github.com/michalin/mousecam
 *
 * @param data Byte to send to the sensor
 */
void photosensor_send_byte(uint8_t data);

/**
 * Receives a single byte from the sensor
 * Note: Function written with reference to https://github.com/michalin/mousecam
 *
 * @return Byte received from the sensor
 */
uint8_t photosensor_receive_byte();

/**
 * Function to read the value of a register from the sensor
 *
 * @param reg Register address to read
 * @return Value stored at that register
 */
uint8_t photosensor_read_register(uint8_t reg);

/**
 * Write a byte to a register in the sensor. This does not check write permissions
 *
 * @param reg Register to write the data to
 * @param data Byte to write into the register
 */
void photosensor_write_register(uint8_t reg, uint8_t data);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PHOTOSENSOR_H_