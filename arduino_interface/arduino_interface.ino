#include "Planimeter.hpp"

Planimeter* planimeter;

void setup() {
  planimeter = new Planimeter(128, 32, &Wire1, -1);

  Serial.begin(9600);
  
}

void loop() {
  // planimeter->updatePosition();
  // planimeter->runGreenesAccumulator();
  // planimeter->drawDisplay();

  // planimeter->drawDisplay();
  planimeter->runPlanimeter();
  
}
