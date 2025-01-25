#define INTER_PULSE_DELAY_MS 10
#define INTER_BYTE_DELAY_MS 100
#define INTER_CLOCK_DELAY_MS 10

// Pins for digital inputs
const int MOTION_PIN = A1;
const int SDIO = 5;
const int SCLK = 6;

void setup() {
  pinMode(MOTION_PIN, INPUT);
  pinMode(SDIO, OUTPUT);
  pinMode(SCLK, OUTPUT);
  digitalWrite(SCLK, HIGH);
  
  // Start Serial USB communication
  Serial.begin(9600);

  while (!Serial) {}

  Serial.println("KB2040 Serial USB ready!");
}

void loop() {
  Serial.printf("0x09: 0x%x\n", read_register(0x09));
  Serial.printf("Writing 0x5A\n");
  write_register(0x09, 0x5A);
  Serial.printf("0x09: 0x%x\n", read_register(0x09));
  delay(50000);

  // int motion = reg_read(0x02);
  // int8_t x = reg_read(0x03);
  // int8_t y = reg_read(0x04);
  // Serial.printf("Motion: %d; Delta: (%d, %d)\n", motion, x, y);
  // delay(100);

  // send_byte(0x55);
  // write_register(0x10, 0xa);
  // delay(10000);
  // Serial.printf("0x00: %x\n", reg_read(0x00));
  // delay(1000);
  // delay(100);
  // Serial.println("Reg?");

  // while (Serial.available() <= 0) {}
  // uint8_t reg = Serial.readString().toInt();

  // Serial.printf("0x%x: r/w?\n", reg);
  // while (Serial.available() <= 0) {}
  // bool read = strncmp(Serial.readString().c_str(), "r", 1) == 0;

  // // resync();
  // if (read) {    
  //   Serial.printf("Data: 0x%x\n", reg_read(reg));
  // }
  // else {
  //   Serial.println("Data?");
  //   while (Serial.available() <= 0) {}
  //   uint8_t data = Serial.readString().toInt();
  //   Serial.printf("Sending: 0x%x\n", data);
  //   write_register(reg, data);
  // }

}

uint8_t read_register(uint8_t reg) {
  // Since we're reading we have to mark the MSB as 0
  // That should already be the case with reg, but I'm annoying and will do it again
  reg &= ~0x80;
  send_byte(reg);

  // Give the chip time to think
  delayMicroseconds(INTER_BYTE_DELAY_MS);

  // Then we receive
  return receive_byte();
  
}

// Written using reference from Doctor Volt (https://www.youtube.com/watch?v=qAlpt_XYkXI)
void send_byte(uint8_t data) {
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

// Written using reference from Doctor Volt (https://www.youtube.com/watch?v=qAlpt_XYkXI)
uint8_t receive_byte() {
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

void write_register(uint8_t reg, uint8_t data) {
  // We send the register with a 1 as MSB then we send the data normally after no delay
  send_byte(reg | 0x80);
  send_byte(data);
}
