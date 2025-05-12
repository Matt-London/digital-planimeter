#include "Wire.h"
#include <Adafruit_SSD1306.h>

void setup() {
  // put your setup code here, to run once:
  Wire.begin();

  Adafruit_SSD1306 display(128, 32, &Wire, -1);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(0);
  display.clearDisplay();

  // Draw border
  display.drawRect(0, 0, display.width(), display.height(), SSD1306_WHITE);

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Test");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
