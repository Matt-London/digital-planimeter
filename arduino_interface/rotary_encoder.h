#ifndef __ROTARY_ENCODER_H_
#define __ROTARY_ENCODER_H_

#include <Arduino.h>

// Pins for digital inputs
const int ENCODER_PIN_A = A1;
const int ENCODER_PIN_B = A0;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Run necessary functions to initiate communication over pins
 */
void rotary_encoder_setup();

/**
 * Read the rotary encoder
 * Note: Function written with reference to https://forum.arduino.cc/t/reading-rotary-encoders-as-a-state-machine/937388
 *
 * @return State of encoder (-1: counter-clockwise, 0: no movement, 1: clockwise)
 */
int8_t rotary_encoder_read();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __ROTARY_ENCODER_H_