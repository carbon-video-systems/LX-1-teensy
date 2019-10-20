#include "led.h"

void setup() {
    generateRainbow();
    led_begin();
    delay(500);
}

void loop() {

//   setAllColour(RED, 1000);
//   setAllColour(GREEN, 1000);
//   setAllColour(BLUE, 1000);
//   setAllColour(YELLOW, 1000);
//   setAllColour(PINK, 1000);
//   setAllColour(ORANGE, 1000);
//   setAllColour(WHITE, 1000);
    rainbow(500);
}
