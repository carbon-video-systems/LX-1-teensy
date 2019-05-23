/* Includes ---------------------------------------------------------------------------------------*/
#include "ODriveLib.h"
#include "calibration.h"

#include <Arduino.h>

/* Constants --------------------------------------------------------------------------------------*/
#define TESTING
#define odrive_serial   Serial2
#define numMotors       2

// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

/* Variables --------------------------------------------------------------------------------------*/
ODriveClass odrive(odrive_serial);

/* Functions --------------------------------------------------------------------------------------*/
/**
 * @brief   The Arduino setup function
 * @param   None
 * @return  None
 */
void setup() {
    // ODrive uses 115200 baud
    odrive_serial.begin(115200);
    while(!odrive_serial);

    #ifdef TESTING
        // Serial to PC
        Serial.begin(115200);
        while (!Serial); // wait for Arduino Serial Monitor to open]
    #endif

    int32_t current_state;
    // search for post-startup ODrive
    for (int axis = 0; axis < numMotors; axis++) {
        current_state = odrive.readState(axis);
        while(current_state != ODriveClass::AXIS_STATE_CLOSED_LOOP_CONTROL || current_state != ODriveClass::AXIS_STATE_IDLE){
            delay(100);
            current_state = odrive.readState(axis);
        }
    }

    while (odrive.readState(0) != ODriveClass::AXIS_STATE_CLOSED_LOOP_CONTROL || ){

    }
    // calibration check + sequence

    delay(100);
}

/**
 * @brief   The Arduino loop function
 * @param   None
 * @return  None
 */
void loop() {
    delay(500);
}