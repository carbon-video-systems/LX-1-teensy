#include "led.h"

void setup() {
    led_begin();
    delay(5000);
}

void loop() {
//   int microsec = 2000000 / leds.numPixels();  // change them all in 2 seconds

  // uncomment for voltage controlled speed
  // millisec = analogRead(A9) / 40;

  setAllColour(RED, 1000);
  setAllColour(GREEN, 1000);
  setAllColour(BLUE, 1000);
  setAllColour(YELLOW, 1000);
  setAllColour(PINK, 1000);
  setAllColour(ORANGE, 1000);
  setAllColour(WHITE, 1000);
}
