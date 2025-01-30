// Pins for analog inputs
const int pinA1 = A1;
const int pinA2 = A2;
const int pinA3 = A3;

void setup() {
  pinMode(pinA1, INPUT);
  pinMode(pinA2, INPUT);
  pinMode(pinA3, INPUT);
  
  // Start Serial USB communication
  Serial.begin(9600);
  while (!Serial) {
    // Wait for the serial port to connect (needed for boards like KB2040)
  }
  Serial.println("KB2040 Serial USB ready!");
}

void loop() {
  // Read analog values from pins
  int valueA1 = digitalRead(pinA1);
  int valueA2 = digitalRead(pinA2);
  int valueA3 = digitalRead(pinA3);

  // Send the values over Serial USB
  Serial.print("A1: ");
  Serial.print(valueA1);
  Serial.print(" | A2: ");
  Serial.print(valueA2);
  Serial.print(" | A3: ");
  Serial.println(valueA3);

  // Add a small delay for readability
  // delay(500);
}
