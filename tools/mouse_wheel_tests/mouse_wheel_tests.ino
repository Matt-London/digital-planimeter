/* Read Quadrature Encoder
   Connect Encoder to Pins encoder0PinA, encoder0PinB, and +5V.

   Sketch by max wolf / www.meso.net
   v. 0.1 - very basic functions - mw 20061220

*/

int val;
int encoder0PinA = D0;
int encoder0PinB = D1;
int encoder0Pos = 0;
int lastNum = 0;
int n = LOW;
int totalNum = 0;

void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  Serial.begin (9600);
}

void loop() {
  int pinA = digitalRead(encoder0PinA);
  int pinB = digitalRead(encoder0PinB);
  int num = (pinA << 1) + pinB;
  int mov = 0;

  if (num == lastNum) {
    mov = 0;
  }
  else if (num < lastNum) {
    mov = -1;
  }
  else {
    mov = 1;
  }

  lastNum = num;

  totalNum += mov;
  

  Serial.println(num);
}

// const int PIN_A = D0;
// const int PIN_B = D1;

// void setup() {
//   pinMode(PIN_A, INPUT_PULLUP);
//   pinMode(PIN_B, INPUT_PULLUP);

//   Serial.begin();

// }

// void loop() {
//   Serial.printf("A: %d, B: %d, ENC: %d\n", digitalRead(PIN_A), digitalRead(PIN_B), rotary_encoder_read());

// }


// int8_t rotary_encoder_read() {
//   bool pin_a_state = digitalRead(PIN_A);
//   bool pin_b_state = digitalRead(PIN_B);


// }

// // int8_t rotary_encoder_read() {
// //   static uint8_t state = 0;
// //   bool pin_a_state = digitalRead(PIN_A);
// //   bool pin_b_state = digitalRead(PIN_B);
// //   switch (state) {
// //     case 0:                         // Idle state, encoder not turning
// //       if (!pin_a_state){             // Turn clockwise and CLK goes low first
// //         state = 1;
// //       } else if (!pin_b_state) {      // Turn anticlockwise and DT goes low first
// //         state = 4;
// //       }
// //       break;
// //     // Clockwise rotation
// //     case 1:                     
// //       if (!pin_b_state) {             // Continue clockwise and DT will go low after CLK
// //         state = 2;
// //       } 
// //       break;
// //     case 2:
// //       if (pin_a_state) {             // Turn further and CLK will go high first
// //         state = 3;
// //       }
// //       break;
// //     case 3:
// //       if (pin_a_state && pin_b_state) {  // Both CLK and DT now high as the encoder completes one step clockwise
// //         state = 0;
// //         return 1;
// //       }
// //       break;
// //     // Anticlockwise rotation
// //     case 4:                         // As for clockwise but with CLK and DT reversed
// //       if (!pin_a_state) {
// //         state = 5;
// //       }
// //       break;
// //     case 5:
// //       if (pin_b_state) {
// //         state = 6;
// //       }
// //       break;
// //     case 6:
// //       if (pin_a_state && pin_b_state) {
// //         state = 0;
// //         return -1;
// //       }
// //       break; 
// //   }
// //   return 0;
// // }