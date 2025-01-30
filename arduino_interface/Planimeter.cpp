#include "Planimeter.hpp"
#include "photosensor.h"

Planimeter::Planimeter(int length, int width, TwoWire* wire, int reset_pin) : display(length, width, wire, reset_pin) {
  photosensor_setup();

  // Rotary encoder setup
  pinMode(ENCODER_PINA, INPUT_PULLUP);
  pinMode(ENCODER_PINB, INPUT_PULLUP);

  // Buttons
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  this->mode = CalibrateInput;

  wire->setSDA(SDA_PIN);
  wire->setSCL(SCL_PIN);
  wire->begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.setRotation(2);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
}

void Planimeter::runGreenesAccumulator() {
  int dx = this->currentPoint.x - this->lastPoint.x;
  int dy = this->currentPoint.y - this->lastPoint.y;

  // Serial.printf("dx=%d; dy=%d; x=%d; y=%d\n", dx, dy, this->currentPoint.x, this->currentPoint.y);

  this->area += ((this->currentPoint.x * dy) - (this->currentPoint.y * dx)) / 2;
  this->lastPoint.x = this->currentPoint.x;
  this->lastPoint.y = this->currentPoint.y;
}

void Planimeter::drawAreaDisp() {
  // Figure out unit string
  // String unit_str = "";
  // switch (unit) {
  //   case Inches:
  //     unit_str = "in";
  //     break;
  //   case Meters:
  //     unit_str = "m";
  //     break;
  //   case Centimeters:
  //     unit_str = "cm";
  //     break;
  // }

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(60, 0);
  display.println("Trace shape");
  // display.drawFastHLine(0, 7, 128, SSD1306_WHITE);
  display.setTextSize(3);
  display.print((int) round(area));
  display.print(" ");
  // display.print(unit_str);
  display.setTextSize(1);
  display.print("2");
  display.display();
  // delay(250);
}

void Planimeter::drawWaitingDisp() {

}

void Planimeter::drawCalibrateInputDisp() {

}

void Planimeter::drawCalibrateTraceDisp() {

}

void Planimeter::drawPositionDisp() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  double area_in = sq(5 / 4359) * this->area;
  display.printf("A: %.2fin^2\n", area_in);
  display.setTextSize(1);
  display.printf("(%d, %d)\n", this->currentPoint.x, this->currentPoint.y);
  display.display();
}

void Planimeter::drawDisplay() {
  switch (this->mode) {
    case Area:
      this->drawAreaDisp();
      break;
    case Waiting:
      this->drawWaitingDisp();
      break;
    case CalibrateInput:
      this->drawCalibrateInputDisp();
      break;
    case CalibrateTrace:
      this->drawCalibrateTraceDisp();
      break;
    case Position:
      this->drawPositionDisp();
      break;
  }
}

void Planimeter::updatePosition() {
  // Get position
  int motion = photosensor_read_register(0x02);
  int8_t dx = photosensor_read_register(0x03);
  int8_t dy = photosensor_read_register(0x04);

  // Update accumulator
  this->currentPoint.x += dx;
  this->currentPoint.y += dy;
}

void Planimeter::runPlanimeter() {
  // Do calibration screen
  int encoder_pos = 0;
  int encoder_last = LOW;

  while (digitalRead(BUTTON1_PIN) == HIGH) {
    // Read encoder A and B signals
    int currA = digitalRead(ENCODER_PINA);
    int currB = digitalRead(ENCODER_PINB);

    // Check for rotation (handles direction)
    if (currA != encoder_last) {
      if (currB != currA) {
        encoder_pos++; // Clockwise
      }
      else {
        if (encoder_pos > 0) {
          encoder_pos--; // Counterclockwise
        }
      }
      encoder_last = currA; // Update last state
      
      // Debounce delay
      delay(5);
    }

    // Update the display
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Calibration length:");
    display.setTextSize(2);
    display.printf("%d un", encoder_pos);
    display.display();

  }

  this->calibratedDistanceReal = encoder_pos;
  encoder_pos = 0;

  while (digitalRead(BUTTON1_PIN) == LOW);

  // Now select a unit
  while (digitalRead(BUTTON1_PIN) == HIGH) {
    // Read encoder A and B signals
    int currA = digitalRead(ENCODER_PINA);
    int currB = digitalRead(ENCODER_PINB);

    // Check for rotation (handles direction)
    if (currA != encoder_last) {
      if (currB != currA) {
        encoder_pos++; // Clockwise
      } else {
        if (encoder_pos > 0) {
          encoder_pos--; // Counterclockwise
        }
      }
      encoder_last = currA; // Update last state
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Calibration unit:");
    display.setTextSize(2);

    int unit_index = (encoder_pos % AVAILABLE_UNITS_SIZE + AVAILABLE_UNITS_SIZE) % AVAILABLE_UNITS_SIZE;
    display.printf("%d ", this->calibratedDistanceReal);
    display.print(AVAILABLE_UNITS[unit_index].c_str());
    display.setTextSize(1);
    display.printf("2");
    display.display();

    delay(3);
  }

  this->unit_index = unit_index;

  display.clearDisplay();
  display.setCursor(0, 0);

  display.println("Done.");
  display.display();

  while (true);

}
