#ifndef __PLANIMETER_HPP_
#define __PLANIMETER_HPP_

#include "Wire.h"
#include <Adafruit_SSD1306.h>
#include "point.h"


#define MAX_POINTS_SUPPORTED 1

const int SDA_PIN = D10;
const int SCL_PIN = MOSI;
const int ENCODER_PINA = A1;
const int ENCODER_PINB = A0;
const int BUTTON1_PIN = A3;
const int BUTTON2_PIN = A2;
const String AVAILABLE_UNITS[] = {"in", "ft", "mi", "ac", "m", "cm", "mm", "km"};
const int AVAILABLE_UNITS_SIZE = 8;

enum DisplayMode {
  Area,             /// Displays the resulting area
  Waiting,          /// Waiting for a new trace
  CalibrateInput,   /// Setting calibration unit
  CalibrateTrace,   /// Tracing the calibration point
  Position,         /// Displays position accumulators for testing
};

class Planimeter {
private:
  Adafruit_SSD1306 display;

  /// What function is being displayed
  DisplayMode mode;

  /// The calculated area so far
  int area = 0;

  /// Position accumulator
  point_t currentPoint = {0, 0};

  // Previous point
  point_t lastPoint = {0, 0};

  /// Delta x and Delta y for greene's theorem
  // point_t dPoint;

  /// What unit we are calibrated in
  int unit_index = 0;

  /// The distance in pixels for calibration length
  int calibratedDistancePixel;

  /// The distance in real life units for calibration
  int calibratedDistanceReal;

  /// All points seen within this iteration
  point_t points[MAX_POINTS_SUPPORTED];

  void drawAreaDisp();
  void drawWaitingDisp();
  void drawCalibrateInputDisp();
  void drawCalibrateTraceDisp();
  void drawPositionDisp();

public:
  Planimeter(int length, int width, TwoWire* wire, int reset_pin);
  Planimeter();

  void runGreenesAccumulator();

  void updatePosition();

  void drawDisplay();

  void runPlanimeter();

};

#endif // __PLANIMETER_HPP_