/* Includes ---------------------------------------------------------------------------------------*/
#include <Arduino.h>
#include "options.h"

#include "ODriveLib.h"
#include "calibration.h"

/* Constants --------------------------------------------------------------------------------------*/

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
        SerialUSB.begin(115200);
        while (!Serial); // wait for Arduino Serial Monitor to open]
    #endif

    odrive_startup_sequence(odrive);

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