#include <cmath>
#include "Planimeter.hpp"
#include "photosensor.h"
#include "rotary_encoder.h"

Planimeter::Planimeter(int length, int width, TwoWire* wire, int reset_pin) : display(length, width, wire, reset_pin) {
  photosensor_setup();
  rotary_encoder_setup();

  // Buttons
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  this->mode = Position;

  // This is not needed (and doesn't work!) for Seeed Xiao esp32c6
  // wire->setSDA(SDA_PIN);
  // wire->setSCL(SCL_PIN);
  wire->begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.setRotation(2);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
}

int Planimeter::calculateAreaSegment(point_t current, point_t last) {
  int dx = current.x - last.x;
  int dy = current.y - last.y;

  return ((current.x * dy) - (current.y * dx)) / 2;
}

int Planimeter::runGreenesAccumulator() {
  // First calculate and add in the previous segment
  this->area += this->calculateAreaSegment(this->currentPoint, this->lastPoint);

  this->lastPoint.x = this->currentPoint.x;
  this->lastPoint.y = this->currentPoint.y;

  // But now we need to return the area as if this is the last point, so we connect to the first point
  return this->area + this->calculateAreaSegment(this->firstPoint, this->lastPoint);

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
  this->updatePosition();
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  double area_in = pow(5 / 4359, 2) * this->area;
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
  // Serial.printf("(%d, %d)\n", this->currentPoint.x, this->currentPoint.y);
}

void Planimeter::printCalInputScreen(String prompt, int num, int unit_idx) {
  // See if we should print unit squared or a real unit
  String unit;
  if (unit_idx == -1) {
    unit = "un";
  }
  else {
    unit = AVAILABLE_UNITS[unit_idx];
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.printf("%s\n\n", prompt.c_str());
  display.setTextSize(2);
  display.printf("%d %s", num, unit.c_str());
  // display.setTextSize(1);
  // display.printf("2");
  display.display();
}

void Planimeter::printInstruction(String instr) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.printf("%s\n", instr.c_str());
  display.display();
}

void Planimeter::printInstructionHalt(String instr) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.printf("%s\n", instr.c_str());
  display.printf("Press button 1 to continue...\n");
  display.display();

  delay(300);
  while (digitalRead(BUTTON1_PIN) == HIGH);
}

void Planimeter::printArea(double area) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.printf("%.2f %s", area, AVAILABLE_UNITS[this->unit_index].c_str());
  display.setTextSize(1);
  display.printf("2\n\n");
  display.printf("(%d, %d)", this->currentPoint.x, this->currentPoint.y);
  display.display();
}

void Planimeter::runPlanimeter() {
  // Put in initial display
  this->printCalInputScreen("Calibration length:", 0, -1);

  // Do calibration screen
  int encoder_pos = 0;
  int encoder_last = LOW;

  while (digitalRead(BUTTON1_PIN) == HIGH) {
    // Get any change in value
    int change = rotary_encoder_read();

    encoder_pos += change;

    if (encoder_pos < 0) {
      encoder_pos = 0;
    }

    if (change != 0) {
      // Update the display
      this->printCalInputScreen("Calibration length:", encoder_pos, -1);
    }
  }

  this->calibratedDistanceReal = encoder_pos;
  encoder_pos = 0;

  delay(100);
  while (digitalRead(BUTTON1_PIN) == LOW);

  this->printCalInputScreen("Calibration unit:", this->calibratedDistanceReal, -1);

  // Now select a unit
  int unit_index;
  while (digitalRead(BUTTON1_PIN) == HIGH) {
    // Get any change in value
    int change = rotary_encoder_read();

    encoder_pos += change;

    if (encoder_pos < 0) {
      encoder_pos = 0;
    }

    if (change != 0) {
      unit_index = (encoder_pos % AVAILABLE_UNITS_SIZE + AVAILABLE_UNITS_SIZE) % AVAILABLE_UNITS_SIZE;
      this->printCalInputScreen("Calibration unit:", this->calibratedDistanceReal, unit_index);
    }

  }

  delay(300);

  this->unit_index = unit_index;

  // display.clearDisplay();
  // display.setCursor(0, 0);

  // display.println("Done.");
  // display.printf("Len: %d, Unit: %s\n", this->calibratedDistanceReal, AVAILABLE_UNITS[this->unit_index].c_str());
  // display.display();

  this->printInstruction("Put cursor on first point");
  while (digitalRead(BUTTON1_PIN) == HIGH) {
    this->updatePosition();
  }
  int first_point_x = this->currentPoint.x;
  int first_point_y = this->currentPoint.y;
  delay(300);


  this->printInstruction("Move cursor to second point");

  while (digitalRead(BUTTON1_PIN) == HIGH) {
    this->updatePosition();
  }

  int second_point_x = this->currentPoint.x;
  int second_point_y = this->currentPoint.y;
  delay(300);

  // Calculate the distance of the line
  this->calibratedDistancePixel = sqrt(pow(second_point_y - first_point_y, 2) + pow(second_point_x - first_point_x, 2));

  while(true) {
    this->area = 0;
    this->printInstruction("Put the cursor on first point and start drawing after click.");
    delay(300);
    while (digitalRead(BUTTON1_PIN) == HIGH) {
      this->updatePosition();
    }
    this->firstPoint = this->currentPoint;
    delay(300);

    while (digitalRead(BUTTON1_PIN) == HIGH) {
      // Update position, accumulate
      this->updatePosition();
      int currArea = this->runGreenesAccumulator();

      // Now calculate area and post it
      double conv_area = pow(((double) this->calibratedDistanceReal) / this->calibratedDistancePixel, 2) * currArea;
      this->printArea(conv_area);
    }
  }

  // char buffer[50];
  // snprintf(buffer, sizeof(buffer), "1: (%d,%d), 2: (%d, %d)", first_point_x, first_point_y, second_point_x, second_point_y);
  // this->printInstructionHalt(String(buffer));
  // while (true);

}
