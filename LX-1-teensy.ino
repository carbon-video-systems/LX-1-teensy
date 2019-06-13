/* Includes ---------------------------------------------------------------------------------------*/
#include <Arduino.h>

#include "calibration.h"
#include "ODriveLib.h"
#include "options.h"
#include "stormbreaker.h"


/* Constants --------------------------------------------------------------------------------------*/

// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

/* Variables --------------------------------------------------------------------------------------*/
ODriveClass odrive(odrive_serial);
StormBreaker thor(odrive);

/* Functions --------------------------------------------------------------------------------------*/
/**
 * @brief   The Arduino setup function
 * @param   None
 * @return  None
 */
void setup()
{
    // ODrive uses 115200 baud
    odrive_serial.begin(ODRIVE_SERIAL_BAUD);
    while(!odrive_serial);

    // Pi uses 115200 baud, 8 data bits, 1 stop bit, no parity
    pi_serial.begin(PI_SERIAL_BAUD, SERIAL_8N1);
    while(!odrive_serial);

    #ifdef TESTING
        // USB uses 9600 baud
        SerialUSB.begin(USB_SERIAL_BAUD);
        while (!SerialUSB); // wait for Arduino Serial Monitor to open
    #endif

    odrive_startup_sequence(odrive);

    pi_serial.println("Hi Raspberry Pi how are you today? :D");

    delay(100);
}

/**
 * @brief   The Arduino loop function
 * @param   None
 * @return  None
 */
void loop()
{
    if(pi_serial.available()){
        // SerialUSB.println(pi_serial.read());
        thor.serviceStormBreaker();
    }

    // delay(100);
}
