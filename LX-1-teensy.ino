/* Includes ---------------------------------------------------------------------------------------*/
#include <Arduino.h>

#include "calibration.h"
#include "debug.h"
#include "ODriveLib.h"
#include "options.h"
#include "stormbreaker.h"

/*Errors-------------------------------------------------------------------------------------------*/
#if (!(((defined BODY) == (defined HEAD)) == (defined BOTH_FOR_TESTING))) || (defined BODY && defined HEAD)
    #error Check your system settings in options.h!  Only one of BODY, HEAD, or BOTH_FOR_TESTING may be defined!
#endif 

#if defined BOTH_FOR_TESTING
    #warning BOTH_FOR_TESTING is defined! Both motors will spin.  Do not flash this in production.
#endif
#if defined TESTING
    #warning TESTING is defined! Do not flash this in production.
#endif

/* Constants --------------------------------------------------------------------------------------*/

// Printing with stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

/* Variables --------------------------------------------------------------------------------------*/
ODriveClass odrive(odrive_serial);
StormBreaker thor(odrive);
Debug debugger(odrive);

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
    while(!pi_serial);

    #ifdef TESTING
        // USB uses 9600 baud
        SerialUSB.begin(USB_SERIAL_BAUD);
        while (!SerialUSB); // wait for Arduino Serial Monitor to open
    #endif

    odrive_startup_sequence(odrive);

    #ifdef TESTING
        SerialUSB.println("Hi computer, how are you today? :D");
        pi_serial.println("Hi Raspberry Pi how are you today? :D");
    #endif

    delay(100);
}

/**
 * @brief   The Arduino loop function
 * @param   None
 * @return  None
 */
void loop()
{
    if(pi_serial.available())
        thor.serviceStormBreaker();

    #ifdef TESTING
        if(SerialUSB.available())
            debugger.serviceDebug();
    #endif
}
