#include "rotary_encoder.h"


void rotary_encoder_setup() {
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
}

/**
 * Read the rotary encoder
 * Note: Function written with reference to https://forum.arduino.cc/t/reading-rotary-encoders-as-a-state-machine/937388
 *
 * @return State of encoder (-1: counter-clockwise, 0: no movement, 1: clockwise)
 */
int8_t rotary_encoder_read() {
  static uint8_t state = 0;
  bool pin_a_state = digitalRead(ENCODER_PIN_A);
  bool pin_b_state = digitalRead(ENCODER_PIN_B);
  switch (state) {
    case 0:                         // Idle state, encoder not turning
      if (!pin_a_state){             // Turn clockwise and CLK goes low first
        state = 1;
      } else if (!pin_b_state) {      // Turn anticlockwise and DT goes low first
        state = 4;
      }
      break;
    // Clockwise rotation
    case 1:                     
      if (!pin_b_state) {             // Continue clockwise and DT will go low after CLK
        state = 2;
      } 
      break;
    case 2:
      if (pin_a_state) {             // Turn further and CLK will go high first
        state = 3;
      }
      break;
    case 3:
      if (pin_a_state && pin_b_state) {  // Both CLK and DT now high as the encoder completes one step clockwise
        state = 0;
        return 1;
      }
      break;
    // Anticlockwise rotation
    case 4:                         // As for clockwise but with CLK and DT reversed
      if (!pin_a_state) {
        state = 5;
      }
      break;
    case 5:
      if (pin_b_state) {
        state = 6;
      }
      break;
    case 6:
      if (pin_a_state && pin_b_state) {
        state = 0;
        return -1;
      }
      break; 
  }
  return 0;
}