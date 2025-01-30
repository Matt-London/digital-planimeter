#include "photosensor.h"

void photosensor_setup() {
  pinMode(MOTION_PIN, INPUT);
  pinMode(SDIO, OUTPUT);
  pinMode(SCLK, OUTPUT);
  digitalWrite(SCLK, HIGH);
}

uint8_t photosensor_read_register(uint8_t reg) {
  // Since we're reading we have to mark the MSB as 0
  // That should already be the case with reg, but I'm annoying and will do it again
  reg &= ~0x80;
  photosensor_send_byte(reg);

  // Give the chip time to think
  delayMicroseconds(INTER_BYTE_DELAY_MS);

  // Then we receive
  return photosensor_receive_byte();
  
}

void photosensor_send_byte(uint8_t data) {
  pinMode(SDIO, OUTPUT);

  for (int i = 7; i >= 0; i--) {
    // Lower clock
    digitalWrite(SCLK, LOW);

    // Send the bit
    digitalWrite(SDIO, (data >> i) & 0x01);

    // Send rising edge
    digitalWrite(SCLK, HIGH);
    
  }

  digitalWrite(SDIO, LOW);
}

uint8_t photosensor_receive_byte() {
  // Initialize for receiving
  pinMode(SDIO, INPUT);

  // Start reading in data
  uint8_t data = 0;

  for (int i = 7; i >= 0; i--) {
    // Send rising edge and wait
    digitalWrite(SCLK, LOW);
    digitalWrite(SCLK, HIGH);

    // Now we should have the byte sitting here
    data |= digitalRead(SDIO) << i;

  }

  return data;
}

void photosensor_write_register(uint8_t reg, uint8_t data) {
  // We send the register with a 1 as MSB then we send the data normally after no delay
  photosensor_send_byte(reg | 0x80);
  photosensor_send_byte(data);
}