/* Includes ---------------------------------------------------------------------------------------*/
#include <Arduino.h>
#include "options.h"

#include "ODriveLib.h"
#include "stormbreaker.h"
#include "calibration.h"

/* Constants --------------------------------------------------------------------------------------*/

// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

/* Variables --------------------------------------------------------------------------------------*/
ODriveClass odrive(odrive_serial);
StormBreaker mjolnir;

/* Functions --------------------------------------------------------------------------------------*/
/**
 * @brief   The Arduino setup function
 * @param   None
 * @return  None
 */
void setup() {
    uint8_t incoming_byte;

    // ODrive uses 115200 baud
    odrive_serial.begin(115200);
    while(!odrive_serial);

    // Pi uses 9600 baud
    pi_serial.begin(PI_SERIAL_BAUD);
    while(!odrive_serial);

    #ifdef TESTING
        // Serial to PC
        SerialUSB.begin(115200);
        while (!Serial); // wait for Arduino Serial Monitor to open]
    #endif

    if(pi_serial.available()){
        mjolnir.servicePiMessage();
    }

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
