#include "ODriveLib.h"

// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

// Variables
#define TESTING
#define odrive_serial Serial1


// ODrive object
ODriveClass odrive(odrive_serial);

void setup() {
  // ODrive uses 115200 baud
  odrive_serial.begin(115200);
  while(!odrive_serial);

  #ifdef TESTING
    // Serial to PC
    Serial.begin(115200);
    while (!Serial); // wait for Arduino Serial Monitor to open]
  #endif
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
}