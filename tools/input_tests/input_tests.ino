/* Read Quadrature Encoder
   Connect Encoder to Pins encoder0PinA, encoder0PinB, and +5V.

   Sketch by max wolf / www.meso.net
   v. 0.1 - very basic functions - mw 20061220
    ---- https://forum.arduino.cc/t/rotary-encoders-with-arduino-uno/939996/2
*/

int val;
int encoder0PinA = A1;
int encoder0PinB = A0;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

int button1 = A3;
int button1_pressed = false;
int button2 = A2;
int button2_pressed = false;

void setup() {
  pinMode (encoder0PinA, INPUT_PULLUP);
  pinMode (encoder0PinB, INPUT_PULLUP);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  Serial.begin (9600);
}

void loop() {
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos++;
    } else {
      encoder0Pos--;
    }
    delay(5);
    Serial.printf("Rotary encoder pos: %d\n", encoder0Pos);
  }
  encoder0PinALast = n;

  if (!button1_pressed && digitalRead(button1) == LOW) {
    Serial.println("Button 1 pushed");
    button1_pressed = true;
  }
  else if (digitalRead(button1) == HIGH) {
    button1_pressed = false;
  }

  if (!button2_pressed && digitalRead(button2) == LOW) {
    Serial.println("Button 2 pushed");
    button2_pressed = true;
  }
  else if (digitalRead(button2) == HIGH) {
    button2_pressed = false;
  }
}